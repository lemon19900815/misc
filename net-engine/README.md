# net engine
a simple network engine based on redis2.8

- The engine work good on linux, but windows not.  
- I can't use socket fd as array index to handle aeFileEvent on windows, must use a container to handle them(sockets)? I have no idea.  

# compile
I use cmake to compile this engine.  
On Linux, the step will help you:  
* cd build  
* cmake ..  
* make  

On Windows, use msvc to compile.  

# description
1. use RingBuffer(based on linux kernel) to handle socket data buffer.  
2. c++11 needed.
