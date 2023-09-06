# How to build and run the Docker image

## Build the docker image

`docker build -t minishell-dev -f path/to/your/Dockerfile .`

## Run the container with the project's directory mounted

`docker run -v ~/Documents/Code/42/42cursus/minishell:/root/minishell -it minishell-dev`




