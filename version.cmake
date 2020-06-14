set(horizon_version "v3.11.1")

find_package(Git)
if(GIT_FOUND)
    execute_process(COMMAND
        "${GIT_EXECUTABLE}" describe --tags
        RESULT_VARIABLE horizon_git_describe_result
        OUTPUT_VARIABLE horizon_git_describe_output
        ERROR_QUIET
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    if(horizon_git_describe_result EQUAL 0)
        set(horizon_version "${horizon_git_describe_output}")
    endif()
endif()

message(STATUS "Building Horizon version ${horizon_version}.")

configure_file("${INPUT_FILE}" "${OUTPUT_FILE}")