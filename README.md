# What is docker-enter

docker-enter is a simple program that let's you access a running Docker container. This is similar to nsenter **BUT** without all the bugs and dependencies. Furthermore docker-enter makes use of the docker itself making it possible to use short ID of the container (2-3 first characters of the ID).

Keep in mind that this tool needs docker installed in order to run. However since you want to enter a container, it's implied that you already have it installed so no fuss.

Demonstration:
````
manos@box:~$ docker run -d ubuntu sleep 9999999
73d6cfac58c1b8ade19ec485b249f5a7bf44840bdeeebf36cd675f05625b47ab
manos@box:~$ sudo docker-enter 73d
[sudo] password for manos: 
root@73d6cfac58c1:/# 
````


# Compile
Compilation doesn't need anything more than GCC installed. On Ubuntu:
```
sudo apt-get install gcc
gcc docker-enter.c -o docker-enter
```

# Run
To run you just need to provide the container ID. Notice that it
even accepts the short ID of a container.

```
sudo ./docker-enter [-u <user>] [-d <directory>] <container ID>
```

# Install
With the previous steps you can use docker-enter by giving the full or relative path to it every time. If you however want to be able and use it from anywhere on your computer you probably want to move the binary to `/usr/bin`.

```
sudo mv ./docker-enter /usr/bin
```

Once that is done, you can open a terminal and run docker-enter directly.
