## 去掉仓库历史版本   
   
清空一些历史提交减小仓库大小     
1、确保你的工作目录是干净的，没有未提交的更改：     
git status   
2、创建一个新的孤儿分支，这会创建一个没有父提交的分支：      
git checkout --orphan new-branch   
   
2、添加所有你想要保留的文件到暂存区：      
git add -A   
   
3、提交你刚刚添加的文件，这样就会创建一个新的初始提交：      
git commit -m "Initial commit"   
   
4、删除旧的分支（如果不再需要）：     
git branch -d old-branch   
   
5、如果你想要强制删除旧的分支，可以使用 -D 选项。    
git push origin new-branch --force