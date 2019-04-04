
set(${PROJECT_NAME}_Viz_FilterHandlers_HDRS
  ${${PROJECT_NAME}_SOURCE_DIR}/SIMPLVtkLib/Visualization/FilterHandlers/AbstractFilterHandler.h
)

set(${PROJECT_NAME}_Viz_FilterHandlers_SRCS
  ${${PROJECT_NAME}_SOURCE_DIR}/SIMPLVtkLib/Visualization/FilterHandlers/AbstractFilterHandler.cpp
)

cmp_IDE_SOURCE_PROPERTIES( "${PROJECT_NAME}/FilterHandlers" "${${PROJECT_NAME}_Viz_FilterHandlers_HDRS}" "${${PROJECT_NAME}_Viz_FilterHandlers_SRCS}" "0")