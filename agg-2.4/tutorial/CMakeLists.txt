add_executable(antigrain_tutorial antigrain_tutorial.cpp)
target_link_libraries(antigrain_tutorial antigrain)

if (AGG_USE_AGG2D)
	add_executable(agg2d_tutorial agg2d_tutorial.cpp)
	target_link_libraries(agg2d_tutorial agg2d antigrain)
endif ()