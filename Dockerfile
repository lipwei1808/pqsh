# To Build: docker build --no-cache -t pbui/cse-30341-fa20-project01 . < Dockerfile

FROM	    ubuntu:20.04
MAINTAINER  Peter Bui <pbui@nd.edu>

ENV	    DEBIAN_FRONTEND=noninteractive

RUN	    apt-get update -y

# Run-time dependencies
RUN	    apt-get install -y build-essential python3 gawk valgrind
