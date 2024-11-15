# git知识点

- content
  - [1. git修改默认编辑器](#1. git修改默认编辑器)
  - [2. git submodule](#2. git submodule)
  - [3. git rebase](#3. git rebase)
  - [4. git log](#4. git log)
  - [5. git remote](#5. git remote)
  - [6. git stash](#6. git stash)
  - [7. git branch](#7. git branch)



## 1. git修改默认编辑器

```sh
# git默认使用的时nano编辑器
# 修改为默认vim编辑
vim ~/.gitconfig

# 在文件中增加以下内容
[core]
	editor = vim
```

## 2. git submodule

```shell
# 同步子模块
$ git submodule init
$ git submodule sync
$ git submodule update

# 工程中添加子模块
$ cd submodule
$ git submodule add repo-url

# 操作完成后，可以看到submodule下已经添加了子模块以及.gitmodules(上一级目录下)

# 下载仓库和依赖的子模块
$ git clone repo --recursive
```

您可以使用git中的子模块来执行此操作。在您的存储库中，执行以下操作：

```sh
$ git submodule add path_to_repo path_where_you_want_it
```

因此，如果库的存储库的URL为，`git://github.com/example/some_lib.git`而您想`lib/some_lib`在项目中使用它，则应输入：

```sh
$ git submodule add git://github.com/example/some_lib.git lib/some_lib
```

请注意，这需要从存储库的顶级目录中完成。因此，不要`cd`进入将其放在第一位的目录中。

添加子模块后，或者每当有人对存储库进行新签出时，您需要执行以下操作：

```sh
$ git submodule init
$ git submodule update
```

然后，您添加的所有子模块都将以您拥有的相同版本签出。

当您要更新其中一个库的较新版本时，请`cd`进入子模块并拉出：

```sh
$ cd lib/some_lib
$ git pull
```

然后，当您执行a时`git status`，应该会看到`lib/somelib`修改部分中列出的内容。添加该文件，提交，您就是最新的。当协作者将该提交提交到他们的存储库中时，他们将被`lib/somelib`视为已修改，直到`git submodule update`再次运行。

## 3. git rebase

```shell
# 参考：https://blog.csdn.net/qq_39253370/article/details/124277214?d=1676020974837

# git变基操作（合并多个commit）
$ git rebase -i HEAD~4

# **重要**：变基之后不能直接使用git pull操作，要使用git pull --rebase，否则变基之前的多个commit将再次出现
# 变基的第一条记录需要使用pick，其他的使用s
$ git pull --rebase
```

## 4. git log

```shell
# 一行日志：--pretty=oneline
$ git log --pretty=oneline

# 简短hash：--abbrev-commit
$ git log --abbrev-commit

# 一行子日志简短的hash
$ git log --pretty=oneline --abbrev-commit
$ git log --oneline
```

## 5. git remote

```sh
# 显示git远端地址（显示本地和远程的所有仓库）
$ git remote -v

# fork远程仓库到自己的本地仓库中
remote_git
user_git: fork from remote_git

# 克隆本地仓库
$ git clone @user_git

# 添加远程仓库
$ git remote add upstream @remote_git

# 这时git remote -v可以看到有2个仓库信息

# 从remote仓库拉取更新
$ git pull upstream master

# 从user仓库拉取更新
$ git pull origin master

# 推送提交到user仓库
# git add * & git commit -m "this is a test commit."
$ git push origin master

# git仓库发生迁移
# 1.删除本地对应的fork仓库
$ git remote remove/rm orgin <url>
# 2.添加新的地址仓库
$ git remote add orgin <url>
# 3.直接更换repository
$ git remote set-url origin <url>
```

## 6. git stash

拉取远程仓库时，由于本地存在修改可能拉取会失败，通常的做法是把本地修改提交到暂存区，待远程仓拉取完成后，再从暂存区把本地修改恢复出来。

```sh
# 提交到暂存区
$ git stash

# 从暂存区恢复
$ git stash pop [stash@{n}]

# 查看暂存区列表
$ git stash list
stash@{0}: WIP on dev: 1316032 message1
stash@{1}: WIP on malab: 11e9ca1 test1
stash@{2}: WIP on malab: e987bab test2
stash@{3}: WIP on malab: 59ed354 test3

# 删除最新的 stash 条目
$ git stash drop

# 删除指定的stash条目
$ git stash drop stash@{n}

# 清空所有 stash 条目
$ git stash clear
```

## 7. git branch

```sh
# 查看分支列表
$ git branch

# 新建分支
$ git branch branch_name

# 删除分支，如果有未合并的提交会失败，
# 如果确认不需要，则可使用参数-D强制删除
$ git branch -d branch_name

# 删除远程分支
$ git push origin --delete branch_name

# 切换分支
$ git checkout branch_name

# 新建分支并切换到对应分支
$ git checkout -b branch_name
```

