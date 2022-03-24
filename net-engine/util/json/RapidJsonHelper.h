#ifndef __JSON_OBJ_H__
#define __JSON_OBJ_H__

#include "rapidjson/writer.h"
#include "rapidjson/pointer.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/document.h"
#include "rapidjson/rapidjson.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/istreamwrapper.h"

#include <fstream>
#include <cstdint>
#include <functional>
#include <vector>

using namespace rapidjson;

namespace util
{
  class JsonObj final
  {
  public:
    JsonObj(void) {
      doc_.SetObject();
    }

    JsonObj(const Document& doc) {
      doc_.CopyFrom(doc, doc_.GetAllocator());
    }

    bool Parse(const std::string& json_str) {
      doc_.Parse(json_str.c_str());
      if (doc_.HasParseError())
        return false;
      return true;
    }

    bool ParseFile(const std::string file_path) {
      std::ifstream ifs(file_path);
      IStreamWrapper isw(ifs);
      doc_.ParseStream(isw);
      if (doc_.HasParseError())
        return false;
      return true;
    }

    bool SaveFile(const std::string file_path) {
      std::ofstream ofs;
      ofs.open(file_path, std::ofstream::out);

      if (ofs.is_open())
      {
        auto pretty_str = ToPrettyString();

        ofs << pretty_str.c_str();
        ofs.close();

        return true;
      }

      return false;
    }

    //obj ops
    void Set(const std::string key, const std::string value) {
      Value tmp_key;
      tmp_key.SetString(key.c_str(), key.length(), doc_.GetAllocator());
      Value tmp_val;
      tmp_val.SetString(value.c_str(), value.length(), doc_.GetAllocator());
      doc_.AddMember(tmp_key, tmp_val, doc_.GetAllocator());
    }

    void Set(const std::string key, const char* value) {
      Value tmp_key;
      tmp_key.SetString(key.c_str(), key.length(), doc_.GetAllocator());
      Value tmp_val(kNullType);
      if (value != NULL)
        tmp_val.SetString(value, strlen(value), doc_.GetAllocator());
      doc_.AddMember(tmp_key, tmp_val, doc_.GetAllocator());
    }

    void Set(const std::string key, bool value) {
      SetT<bool>(key, value);
    }

    void Set(const std::string key, int value) {
      SetT<int>(key, value);
    }

    void Set(const std::string key, uint32_t value) {
      SetT<uint32_t>(key, value);
    }

    void Set(const std::string key, uint64_t value) {
      SetT<uint64_t>(key, value);
    }

    void Set(const std::string key, int64_t value) {
      SetT<int64_t>(key, value);
    }

    void Set(const std::string key, double value) {
      Value tmp_key;
      tmp_key.SetString(key.c_str(), key.length(), doc_.GetAllocator());
      Value tmp_val(value);
      doc_.AddMember(tmp_key, tmp_val, doc_.GetAllocator());
    }

    void Set(const std::string key, const JsonObj& value) {
      Value tmp_key;
      tmp_key.SetString(key.c_str(), key.length(), doc_.GetAllocator());
      Value tmp_val;
      tmp_val.CopyFrom(const_cast<JsonObj*>(&value)->GetDocument(), doc_.GetAllocator());
      if (this == &value && !doc_.IsObject())
        doc_.SetObject();
      doc_.AddMember(tmp_key, tmp_val, doc_.GetAllocator());
    }

    bool Get(const std::string key, std::string& value) {
      if (!doc_.HasMember(key.c_str()))
        return false;
      Value& tmp_val = doc_[key.c_str()];
      if (!tmp_val.IsString())
        return false;
      value = std::string(tmp_val.GetString(), tmp_val.GetStringLength());
      return true;
    }

    bool Get(const std::string key, bool& value) {
      if (!doc_.HasMember(key.c_str()))
        return false;
      Value& tmp_val = doc_[key.c_str()];
      if (!tmp_val.IsBool())
        return false;
      value = tmp_val.GetBool();
      return true;
    }

    bool Get(const std::string key, int& value) {
      if (!doc_.HasMember(key.c_str()))
        return false;
      Value& tmp_val = doc_[key.c_str()];
      if (!tmp_val.IsNumber() && !tmp_val.IsInt())
        return false;
      value = tmp_val.GetInt();
      return true;
    }

    bool Get(const std::string key, uint32_t& value) {
      if (!doc_.HasMember(key.c_str()))
        return false;
      Value& tmp_val = doc_[key.c_str()];
      if (!tmp_val.IsNumber() && !tmp_val.IsUint())
        return false;
      value = tmp_val.GetUint();
      return true;
    }

    bool Get(const std::string key, int64_t& value) {
      if (!doc_.HasMember(key.c_str()))
        return false;
      Value& tmp_val = doc_[key.c_str()];
      if (!tmp_val.IsNumber() && !tmp_val.IsInt64())
        return false;
      value = tmp_val.GetInt64();
      return true;
    }

    bool Get(const std::string key, uint64_t& value) {
      if (!doc_.HasMember(key.c_str()))
        return false;
      Value& tmp_val = doc_[key.c_str()];
      if (!tmp_val.IsNumber() && !tmp_val.IsUint64())
        return false;
      value = tmp_val.GetUint64();
      return true;
    }

    bool Get(const std::string key, double& value) {
      if (!doc_.HasMember(key.c_str()))
        return false;
      Value& tmp_val = doc_[key.c_str()];
      if (!tmp_val.IsNumber() && !tmp_val.IsDouble())
        return false;
      value = tmp_val.GetDouble();
      return true;
    }

    bool Get(const std::string key, JsonObj& value) {
      if (!doc_.HasMember(key.c_str()))
        return false;
      Value& tmp_val = doc_[key.c_str()];
      if (!tmp_val.IsObject() && !tmp_val.IsArray())
        return false;
      value = tmp_val;
      return true;
    }

    template<typename T>
    void Update(const std::string key, T& value);

    //array ops
    void Push(const std::string& value) {
      if (!doc_.IsArray())
        doc_.SetArray();
      Value tmp_val;
      tmp_val.SetString(value.c_str(), value.length(), doc_.GetAllocator());
      doc_.PushBack(tmp_val, doc_.GetAllocator());
    }

    void Push(const char* value) {
      if (!doc_.IsArray())
        doc_.SetArray();
      Value tmp_val(kNullType);
      if (value != NULL)
        tmp_val.SetString(value, strlen(value), doc_.GetAllocator());
      doc_.PushBack(tmp_val, doc_.GetAllocator());
    }

    void Push(bool value) {
      PushT<bool>(value);
    }

    void Push(int value) {
      PushT<int>(value);
    }

    void Push(uint32_t value) {
      PushT<uint32_t>(value);
    }

    void Push(uint64_t value) {
      PushT<uint64_t>(value);
    }

    void Push(int64_t value) {
      PushT<int64_t>(value);
    }

    void Push(double value) {
      PushT<double>(value);
    }

    void Push(const JsonObj& obj) {
      if (!doc_.IsArray())
        doc_.SetArray();
      Value tmp_val;
      tmp_val.CopyFrom(const_cast<JsonObj*>(&obj)->GetDocument(), doc_.GetAllocator());
      doc_.PushBack(tmp_val, doc_.GetAllocator());
    }

    int Size() const {
      if (!doc_.IsArray())
        return -1;
      return doc_.Size();
    }

    bool At(uint32_t index, std::string& value) {
      if (!CheckAt(index))
        return false;
      Value& tmp_val = doc_[index];
      if (!tmp_val.IsString())
        return false;
      value = std::string(tmp_val.GetString(), tmp_val.GetStringLength());
      return true;
    }

    bool At(uint32_t index, bool& value) {
      if (!CheckAt(index))
        return false;
      Value& tmp_val = doc_[index];
      if (!tmp_val.IsBool())
        return false;
      value = tmp_val.GetBool();
      return true;
    }

    bool At(uint32_t index, int& value) {
      if (!CheckAt(index))
        return false;
      Value& tmp_val = doc_[index];
      if (!tmp_val.IsNumber() && !tmp_val.IsInt())
        return false;
      value = tmp_val.GetInt();
      return true;
    }

    bool At(uint32_t index, uint32_t& value) {
      if (!CheckAt(index))
        return false;
      Value& tmp_val = doc_[index];
      if (!tmp_val.IsNumber() && !tmp_val.IsUint())
        return false;
      value = tmp_val.GetUint();
      return true;
    }

    bool At(uint32_t index, int64_t& value) {
      if (!CheckAt(index))
        return false;
      Value& tmp_val = doc_[index];
      if (!tmp_val.IsNumber() && !tmp_val.IsInt64())
        return false;
      value = tmp_val.GetInt64();
      return true;
    }

    bool At(uint32_t index, uint64_t& value) {
      if (!CheckAt(index))
        return false;
      Value& tmp_val = doc_[index];
      if (!tmp_val.IsNumber() && !tmp_val.IsUint64())
        return false;
      value = tmp_val.GetUint64();
      return true;
    }

    bool At(uint32_t index, double& value) {
      if (!CheckAt(index))
        return false;
      Value& tmp_val = doc_[index];
      if (!tmp_val.IsNumber() && !tmp_val.IsDouble())
        return false;
      value = tmp_val.GetDouble();
      return true;
    }

    bool At(uint32_t index, JsonObj& value) {
      if (!CheckAt(index))
        return false;
      Value& tmp_val = doc_[index];
      if (!tmp_val.IsObject() && !tmp_val.IsArray())
        return false;
      value = tmp_val;
      return true;
    }

    void foreach_obj(std::function<void(JsonObj&)> handler);

    template< class T1, class T2>
    bool ParsePair(std::string node_name, std::pair<T1, T2>& pair_data);

    template< class T>
    bool ParseVector(std::string node_name, std::vector<T>& data_vec);

    std::string ToString() {
      StringBuffer sb;
      Writer<StringBuffer> writer(sb);
      doc_.Accept(writer);
      return sb.GetString();
    }

    std::string ToPrettyString() {
      StringBuffer sb;
      PrettyWriter<StringBuffer> writer(sb);
      doc_.Accept(writer);
      return sb.GetString();
    }

    ~JsonObj(void) {
    }

    JsonObj& operator=(JsonObj& obj) {
      doc_.CopyFrom(obj.GetDocument(), doc_.GetAllocator());
      return *this;
    }

  private:
    Document& GetDocument() {
      return doc_;
    }

    JsonObj& operator=(const Value& obj) {
      doc_.CopyFrom(obj, doc_.GetAllocator());
      return *this;
    }

    bool CheckAt(uint32_t index) {
      if (!doc_.IsArray())
        return false;
      if (index < 0 || index >= Size())
        return false;
      return true;
    }

  private:
    template< typename T>
    void SetT(const std::string key, T value);

    template< typename T>
    void PushT(T value);

  private:
    Document doc_;
  };

  void JsonObj::foreach_obj(std::function<void(JsonObj&)> handler) {
    if (doc_.IsArray())
    {
      JsonObj obj;
      for (int32_t i = 0; i < Size(); ++i)
      {
        At(i, obj);
        handler(obj);
      }
    }
    else {
      auto itr = doc_.MemberBegin();
      auto end = doc_.MemberEnd();

      JsonObj obj;
      while (itr != end)
      {
        obj = itr->value;
        handler(obj);
        ++itr;
      }
    }
  }

  template< typename T>
  void JsonObj::SetT(const std::string key, T value)
  {
    Value tmp_key;
    tmp_key.SetString(key.c_str(), key.length(), doc_.GetAllocator());
    Value tmp_val(value);
    doc_.AddMember(tmp_key, tmp_val, doc_.GetAllocator());
  }

  template< typename T>
  void JsonObj::PushT(T value)
  {
    if (!doc_.IsArray())
      doc_.SetArray();
    Value tmp_val(value);
    doc_.PushBack(tmp_val, doc_.GetAllocator());
  }

  template< class T1, class T2>
  bool JsonObj::ParsePair(std::string node_name, std::pair<T1, T2>& pair_data)
  {
    JsonObj obj;
    if (!Get(node_name, obj))
      return false;
    if (obj.Size() != 2)
      return false;
    if (!obj.At(0, pair_data.first))
      return false;
    if (!obj.At(1, pair_data.second))
      return false;
    return true;
  }

  template< class T>
  bool JsonObj::ParseVector(std::string node_name, std::vector<T>& data_vec)
  {
    data_vec.clear();

    JsonObj obj;
    if (!Get(node_name, obj))
      return false;
    for (int i = 0; i < obj.Size(); ++i)
    {
      T tmp_val;
      if (!obj.At(i, tmp_val))
        return false;
      data_vec.push_back(tmp_val);
    }
    return true;
  }

  template<typename T>
  void JsonObj::Update(const std::string key, T& value)
  {
    if (!doc_.HasMember(key.c_str()))
    {
      Set(key, value);
    }
    else
    {
      Value& tmp_val = doc_[key.c_str()];
      tmp_val.Set(value);
    }
  }
}

#endif /* __RAPID_JSON_H__ */
