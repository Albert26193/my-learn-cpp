function install_gtest() {
  local proj_dir="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )/.."
  cd "${proj_dir}/tools/googletest"
  cmake -DCMAKE_INSTALL_PREFIX=${proj_dir}/tools/gtest .

  make install
}

install_gtest