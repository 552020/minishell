# Use the official Ubuntu base image
FROM ubuntu:latest

# Set environment variables to non-interactive (this prevents some prompts)
ENV DEBIAN_FRONTEND=non-interactive

# Run package updates and install tools
RUN apt-get update \
    && apt-get install -y zsh git curl wget vim man build-essential gdb make libssl-dev valgrind libreadline-dev \
    && apt-get clean

# Clone the powerlevel10k theme into the root's home directory
RUN git clone --depth=1 https://github.com/romkatv/powerlevel10k.git /root/powerlevel10k

# Re-add man pages
RUN yes | unminimize

# Copy the zshrc configuration file
COPY zshrc-config /root/.zshrc

# Copy the p10k.zsh configuration file
COPY .p10k.zsh /root/.p10k.zsh

# Set the working directory
WORKDIR /root/minishell

# Switch the default shell to zsh
SHELL ["/usr/bin/zsh", "-c"]

# Start zsh by default
CMD ["zsh"]

