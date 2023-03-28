# git知识点

## 1 git修改默认编辑器

```sh
# git默认使用的时nano编辑器
# 修改为默认vim编辑
vim ~/.gitconfig

# 在文件中增加以下内容
[core]
	editor = vim

```

## 2 git submodule

```shell
# 同步子模块
git submodule init
git submodule sync
git submodule update

# 工程中添加子模块
cd submodule
git submodule add repo-url

# 操作完成后，可以看到submodule下已经添加了子模块以及.gitmodules(上一级目录下)
```

## 3 git rebase

```shell
# 参考：https://blog.csdn.net/qq_39253370/article/details/124277214?d=1676020974837

# git变基操作（合并多个commit）
git rebase -i HEAD~4

# **重要**：变基之后不能直接使用git pull操作，要使用git pull --rebase，否则变基之前的多个commit将再次出现
git pull --rebase
```

## 4 git log

```shell
# 一行日志：--pretty=oneline
git log --pretty=oneline

# 简短hash：--abbrev-commit
git log --abbrev-commit

# 一行子日志简短的hash
git log --pretty=oneline --abbrev-commit
```

