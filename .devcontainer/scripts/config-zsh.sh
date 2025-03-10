#!/usr/bin/env bash

set -e

# check and install oh-my-zsh
function check_install_oh_my_zsh() {
  local oh_my_zsh_target_dir="${HOME}/.oh-my-zsh"
  if [ -d "${oh_my_zsh_target_dir}" ]; then
    printf "%s\n" "oh-my-zsh is already installed."
  else
    printf "%s\n" "oh-my-zsh is not installed. Installing..."
    bash -c "$(wget -O- https://raw.githubusercontent.com/ohmyzsh/ohmyzsh/master/tools/install.sh)" "" --unattended >/dev/null
  fi
  
  return 0
}

# check and install oh-my-zsh-plugins
function check_install_oh_my_zsh_plugins() {
  local oh_my_zsh_dir="${HOME}/.oh-my-zsh"
  local oh_my_zsh_plugins_dir="${HOME}/.oh-my-zsh/custom/plugins"
  if [[ ! -d "${oh_my_zsh_dir}" ]]; then
    printf "%s\n" "oh-my-zsh plugins directory has broken, exit now..."
    return 1
  fi

  declare -A to_install_plugins=(
    ["zsh-syntax-highlighting"]="https://github.com/zsh-users/zsh-syntax-highlighting.git"
    ["zsh-vi-mode"]="https://github.com/jeffreytse/zsh-vi-mode.git"
    ["zsh-autosuggestions"]="https://github.com/zsh-users/zsh-autosuggestions.git"
  )

  for plugin in "${!to_install_plugins[@]}"; do
    if [[ ! -d "${oh_my_zsh_plugins_dir}/${plugin}" ]]; then
      printf "%s\n" "${plugin} is not installed. Installing..."
      git clone "${to_install_plugins[$plugin]}" "${oh_my_zsh_plugins_dir}/${plugin}"
    else
      printf "%s\n" "${plugin} is already installed."
    fi
  done

  return 0
}

# inject zsh configuration
function inject_zsh_configuration() {
  local zshrc_file="${HOME}/.zshrc"
  if [[ ! -f "${zshrc_file}" ]]; then
    printf "%s\n" "zshrc file is not found, exit now..."
    return 1
  fi

  # inject zsh configuration
  cat <<'EOF' >> "${zshrc_file}" 
# ---------------------- oh-my-zsh ----------------
export ZSH=$HOME/.oh-my-zsh

ZSH_THEME="ys"

plugins=(git zsh-syntax-highlighting zsh-autosuggestions docker zsh-vi-mode)

HISTFILE="${ZSH}/cache/.zsh_history"
ZSH_COMPDUMP="${ZSH}/cache/.zcompdump-${SHORT_HOST}-${ZSH_VERSION}"

source $ZSH/oh-my-zsh.sh

# --------------------- zsh -----------------------
ZSH_AUTOSUGGEST_HIGHLIGHT_STYLE='fg=blue'

# --------------------- general -------------------
export TERM="xterm-256color"
export LANG=en_US.UTF-8
export LC_ALL=en_US.UTF-8
export LS_COLORS=${LS_COLORS}:'di=01;35'

#  --------------------- alias --------------------
alias "nv"="nvim"
alias "tm"="tmux"
alias "ll"="ls -l"
alias "la"="ls -al"
EOF

  return 0
}

function change_shell_to_zsh() {
  local target_user="${1:-${USER}}"
  if [[ "${SHELL}" != "$(which zsh)" ]]; then
    sudo chsh -s "$(which zsh)" "${target_user}"
  fi
  return 0
}

check_install_oh_my_zsh \
  && check_install_oh_my_zsh_plugins \
  && inject_zsh_configuration