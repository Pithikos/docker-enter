**Description**
Let's a user enter an already running container. Works similar to nsenter without the bugs and dependancies. It further adds some docker oriented functionality (see below for details).


=======================================================================
             

# Why use this
Besides solving some nsenter issues and being super easy to compile, it further supports:
* Accepts short container ID
* Enter as a specific user
* Enter a specific folder

Keep in mind that this tool needs docker installed in order to run. However since you want to enter a container, it's implied that you already have it installed so no fuss.


# Compile
Compilation doesn't need anything more than GCC installed. On Ubuntu:
```
sudo apt-get install gcc
gcc docker-enter.c -o docker-enter
```


# Run
To run you just need to provide the container ID. Notice that it
even accepts the **short ID** like in docker.
```
./docker-enter [-u <user>] [-d <directory>] <container ID>
```
