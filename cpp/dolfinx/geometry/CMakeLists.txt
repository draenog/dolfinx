set(HEADERS_geometry
  ${CMAKE_CURRENT_SOURCE_DIR}/BoundingBoxTree.h
  ${CMAKE_CURRENT_SOURCE_DIR}/gjk.h
  ${CMAKE_CURRENT_SOURCE_DIR}/dolfin_geometry.h
  ${CMAKE_CURRENT_SOURCE_DIR}/utils.h
  PARENT_SCOPE)

target_sources(dolfinx PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/BoundingBoxTree.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/gjk.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/utils.cpp
)
