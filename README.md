**Description**

Let's a user enter an already running container. Works similar to nsenter without the bugs and dependencies. It further adds some docker oriented functionality (see below for details).


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
sudo ./docker-enter [-u <user>] [-d <directory>] <container ID>
```

# Move the binary
You probably want to move the file to the appropriate folder so that
you can run it directly from a terminal without having to issue the full or relative path of docker-enter each time.

On Ubuntu:
```
sudo mv ./docker-enter $(echo $PATH | cut -f1 -d:)
```

This will move it to the first bin folder found in $PATH. After that you can use docker-enter from wherever in the system.
