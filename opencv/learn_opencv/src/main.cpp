#include "util.h"

#include "test_base.h"
#include "test_kmeans.h"
#include "test_knn.h"
#include "test_dtrees.h"

//#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>

void setVarTypes(const std::string& s, int nvars, std::vector<uchar>& vtypes)
{
  const char* errmsg = "type spec is not correct; it should have format \"cat\", \"ord\" or "
    "\"ord[n1,n2-n3,n4-n5,...]cat[m1-m2,m3,m4-m5,...]\", where n's and m's are 0-based variable indices";
  const char* str = s.c_str();
  int specCounter = 0;

  vtypes.resize(nvars);

  for (int k = 0; k < 2; k++)
  {
    const char* ptr = strstr(str, k == 0 ? "ord" : "cat");
    int tp = k == 0 ? cv::ml::VAR_ORDERED : cv::ml::VAR_CATEGORICAL;
    if (ptr) // parse ord/cat str
    {
      char* stopstring = NULL;

      if (ptr[3] == '\0')
      {
        for (int i = 0; i < nvars; i++)
          vtypes[i] = (uchar)tp;
        specCounter = nvars;
        break;
      }

      if (ptr[3] != '[')
        CV_Error(CV_StsBadArg, errmsg);

      ptr += 4; // pass "ord["
      do
      {
        int b1 = (int)strtod(ptr, &stopstring);
        if (*stopstring == 0 || (*stopstring != ',' && *stopstring != ']' && *stopstring != '-'))
          CV_Error(CV_StsBadArg, errmsg);
        ptr = stopstring + 1;
        if ((stopstring[0] == ',') || (stopstring[0] == ']'))
        {
          CV_Assert(0 <= b1 && b1 < nvars);
          vtypes[b1] = (uchar)tp;
          specCounter++;
        }
        else
        {
          if (stopstring[0] == '-')
          {
            int b2 = (int)strtod(ptr, &stopstring);
            if ((*stopstring == 0) || (*stopstring != ',' && *stopstring != ']'))
              CV_Error(CV_StsBadArg, errmsg);
            ptr = stopstring + 1;
            CV_Assert(0 <= b1 && b1 <= b2 && b2 < nvars);
            for (int i = b1; i <= b2; i++)
              vtypes[i] = (uchar)tp;
            specCounter += b2 - b1 + 1;
          }
          else
            CV_Error(CV_StsBadArg, errmsg);

        }
      } while (*stopstring != ']');
    }
  }

  if (specCounter != nvars)
    CV_Error(CV_StsBadArg, "type of some variables is not specified");
}

using MapType = std::map<cv::String, int>;
static const float MISSED_VAL = 0;
static const int VAR_MISSED = 0;

enum VariableTypes
{
  VAR_NUMERICAL = 0, //!< same as VAR_ORDERED
  VAR_ORDERED = 0, //!< ordered variables
  VAR_CATEGORICAL = 1  //!< categorical variables
};

void decodeElem(const char* token, float& elem, int& type,
  char missch, MapType& namemap, int& counter)
{
  char* stopstring = NULL;
  elem = (float)strtod(token, &stopstring);
  if (*stopstring == missch && strlen(stopstring) == 1) // missed value
  {
    elem = MISSED_VAL;
    type = VAR_MISSED;
  }
  else if (*stopstring != '\0')
  {
    MapType::iterator it = namemap.find(token);
    if (it == namemap.end())
    {
      elem = (float)counter;
      namemap[token] = counter++;
    }
    else
      elem = (float)it->second;
    type = VAR_CATEGORICAL;
  }
  else
    type = VAR_ORDERED;
}


bool loadCSV(const cv::String& filename, int headerLines,
  int responseStartIdx, int responseEndIdx,
  const cv::String& varTypeSpec, char delimiter, char missch)
{
  const int M = 1000000;
  const char delimiters[3] = { ' ', delimiter, '\0' };
  int nvars = 0;
  bool varTypesSet = false;

  auto file = fopen(filename.c_str(), "rt");

  if (!file)
    return false;

  std::vector<char> _buf(M);
  std::vector<float> allresponses;
  std::vector<float> rowvals;
  std::vector<uchar> vtypes, rowtypes;
  std::vector<uchar> vsymbolflags;
  bool haveMissed = false;
  char* buf = &_buf[0];

  int i, ridx0 = responseStartIdx, ridx1 = responseEndIdx;
  int ninputvars = 0, noutputvars = 0;

  cv::Mat tempSamples, tempMissing, tempResponses;
  MapType tempNameMap, nameMap;;
  int catCounter = 1;

  // skip header lines
  int lineno = 0;
  for (;; lineno++)
  {
    if (!fgets(buf, M, file))
      break;
    if (lineno < headerLines)
      continue;
    // trim trailing spaces
    int idx = (int)strlen(buf) - 1;
    while (idx >= 0 && isspace(buf[idx]))
      buf[idx--] = '\0';
    // skip spaces in the beginning
    char* ptr = buf;
    while (*ptr != '\0' && isspace(*ptr))
      ptr++;
    // skip commented off lines
    if (*ptr == '#')
      continue;
    rowvals.clear();
    rowtypes.clear();

    char* token = strtok(buf, delimiters);
    if (!token)
      break;

    for (;;)
    {
      float val = 0.f; int tp = 0;
      decodeElem(token, val, tp, missch, tempNameMap, catCounter);
      if (tp == VAR_MISSED)
        haveMissed = true;
      rowvals.push_back(val);
      rowtypes.push_back((uchar)tp);
      token = strtok(NULL, delimiters);
      if (!token)
        break;
    }

    if (nvars == 0)
    {
      if (rowvals.empty())
        CV_Error(CV_StsBadArg, "invalid CSV format; no data found");
      nvars = (int)rowvals.size();
      if (!varTypeSpec.empty() && varTypeSpec.size() > 0)
      {
        setVarTypes(varTypeSpec, nvars, vtypes);
        varTypesSet = true;
      }
      else
        vtypes = rowtypes;
      vsymbolflags.resize(nvars);
      for (i = 0; i < nvars; i++)
        vsymbolflags[i] = (uchar)(rowtypes[i] == VAR_CATEGORICAL);

      ridx0 = ridx0 >= 0 ? ridx0 : ridx0 == -1 ? nvars - 1 : -1;
      ridx1 = ridx1 >= 0 ? ridx1 : ridx0 >= 0 ? ridx0 + 1 : -1;
      CV_Assert(ridx1 > ridx0);
      noutputvars = ridx0 >= 0 ? ridx1 - ridx0 : 0;
      ninputvars = nvars - noutputvars;
    }
    else
      CV_Assert(nvars == (int)rowvals.size());

    // check var types
    for (i = 0; i < nvars; i++)
    {
      CV_Assert((!varTypesSet && vtypes[i] == rowtypes[i]) ||
        (varTypesSet && (vtypes[i] == rowtypes[i] || rowtypes[i] == VAR_ORDERED)));
      uchar sflag = (uchar)(rowtypes[i] == VAR_CATEGORICAL);
      if (vsymbolflags[i] == VAR_MISSED)
        vsymbolflags[i] = sflag;
      else
        CV_Assert(vsymbolflags[i] == sflag || rowtypes[i] == VAR_MISSED);
    }

    if (ridx0 >= 0)
    {
      for (i = ridx1; i < nvars; i++)
        std::swap(rowvals[i], rowvals[i - noutputvars]);
      for (i = ninputvars; i < nvars; i++)
        allresponses.push_back(rowvals[i]);
      rowvals.pop_back();
    }
    cv::Mat rmat(1, ninputvars, CV_32F, &rowvals[0]);
    tempSamples.push_back(rmat);
  }

  fclose(file);

  int nsamples = tempSamples.rows;
  if (nsamples == 0)
    return false;

  if (haveMissed)
    cv::compare(tempSamples, MISSED_VAL, tempMissing, 0);

  if (ridx0 >= 0)
  {
    for (i = ridx1; i < nvars; i++)
      std::swap(vtypes[i], vtypes[i - noutputvars]);
    if (noutputvars > 1)
    {
      for (i = ninputvars; i < nvars; i++)
        if (vtypes[i] == VAR_CATEGORICAL)
          CV_Error(CV_StsBadArg,
            "If responses are vector values, not scalars, they must be marked as ordered responses");
    }
  }

  if (!varTypesSet && noutputvars == 1 && vtypes[ninputvars] == VAR_ORDERED)
  {
    for (i = 0; i < nsamples; i++)
      if (allresponses[i] != cvRound(allresponses[i]))
        break;
    if (i == nsamples)
      vtypes[ninputvars] = VAR_CATEGORICAL;
  }

  //If there are responses in the csv file, save them. If not, responses matrix will contain just zeros
  //if (noutputvars != 0) {
  //  cv::Mat(nsamples, noutputvars, CV_32F, &allresponses[0]).copyTo(tempResponses);
  //  setData(tempSamples, cv::ml::ROW_SAMPLE, tempResponses, cv::noArray(), cv::noArray(),
  //    cv::noArray(), cv::Mat(vtypes).clone(), tempMissing);
  //}
  //else {
  //  cv::Mat zero_mat(nsamples, 1, CV_32F, cv::Scalar(0));
  //  zero_mat.copyTo(tempResponses);
  //  setData(tempSamples, cv::ml::ROW_SAMPLE, tempResponses, cv::noArray(), cv::noArray(),
  //    cv::noArray(), cv::noArray(), tempMissing);
  //}
  //bool ok = !samples.empty();
  //if (ok)
  //{
  //  std::swap(tempNameMap, nameMap);
  //  cv::Mat(vsymbolflags).copyTo(varSymbolFlags);
  //}
  //return ok;
  return true;
}

int main() {

  //auto varTypeSpec = static_cast<std::string>("cat[0-22]");

  //std::vector<uchar> vtypes;
  //setVarTypes(varTypeSpec, 23, vtypes);

  //std::string csv_filename = "../datasets/mushroom/append_test.data";

  ////1.读取csv文件
  //auto dataset = loadCSV(csv_filename, 0, 0, 1, "cat[0,2-23]ord[1]", ',', '?');

  //testbase::Run();
  //testkmeans::Run();
  //testknn::Run();
  testdtree::Run();

  //int32_t int_min = 0x80000000;
  //int32_t int_max = 0x7FFFFFFF;
  //LOG("int_min = " << int_min << ", int_max = " << int_max);

  //cv::waitKey();

  system("pause");
  return 0;
}
