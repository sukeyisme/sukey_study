#配置文件源码设置和生成目录设置
configure_file (
  "${PROJECT_SOURCE_DIR}/config.h.in"
  "${PROJECT_BINARY_DIR}/config.h"
  )

#导出符号
set (DaoChu 1)