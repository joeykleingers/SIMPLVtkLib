set(SUBDIR_NAME Common)

set(${PROJECT_NAME}_${SUBDIR_NAME}_HDRS

)

set(${PROJECT_NAME}_${SUBDIR_NAME}_HDRS
  ${${PROJECT_NAME}_SOURCE_DIR}/SIMPLVtkLib/${SUBDIR_NAME}/SIMPLVtkLibConstants.h
)

cmp_IDE_SOURCE_PROPERTIES( "${PROJECT_NAME}/${SUBDIR_NAME}" "${${PROJECT_NAME}_${SUBDIR_NAME}_HDRS}" "${${PROJECT_NAME}_${SUBDIR_NAME}_SRCS}" "0")