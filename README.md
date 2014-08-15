=======================================================================

Author: Johan Hanssen Seferidis

Description: Similar to nsenter. The difference is that it is docker
             oriented, thus it avoids some bugs that nsenter might have.
             
             Pros:
             * Accepts short container ID
             * Enter as a specific user
             * Enter a specific folder

             This tools needs docker installed to run. However since
             you want to enter a container, it's implied that you have
             installed docker anyway.

=======================================================================



#Compile
Compilation doesn't need any special flags. Just run:
    gcc docker-enter.c -o docker-enter


#Run
To run you just need to provide the container ID. Notice that it
even accepts the **short ID** like in docker.
    ./docker-enter [-u <user>] [-d <directory>] <container ID>
