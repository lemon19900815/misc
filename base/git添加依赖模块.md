您可以使用git中的子模块来执行此操作。在您的存储库中，执行以下操作：

```
git submodule add path_to_repo path_where_you_want_it
```

因此，如果库的存储库的URL为，`git://github.com/example/some_lib.git`而您想`lib/some_lib`在项目中使用它，则应输入：

```
git submodule add git://github.com/example/some_lib.git lib/some_lib
```

请注意，这需要从存储库的顶级目录中完成。因此，不要`cd`进入将其放在第一位的目录中。

添加子模块后，或者每当有人对存储库进行新签出时，您需要执行以下操作：

```
git submodule init
git submodule update
```

然后，您添加的所有子模块都将以您拥有的相同版本签出。

当您要更新其中一个库的较新版本时，请`cd`进入子模块并拉出：

```
cd lib/some_lib
git pull
```

然后，当您执行a时`git status`，应该会看到`lib/somelib`修改部分中列出的内容。添加该文件，提交，您就是最新的。当协作者将该提交提交到他们的存储库中时，他们将被`lib/somelib`视为已修改，直到`git submodule update`再次运行。