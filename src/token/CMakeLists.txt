target_sources(${TOKEN_LIBNAME} PRIVATE token.cpp)

target_sources(
    ${TOKEN_EXENAME}
    PRIVATE main.cpp
    PRIVATE FILE_SET HEADERS FILES main.hpp)
