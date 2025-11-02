cmake_minimum_required(VERSION 3.20)
project(MMMaster VERSION 1.0.0 LANGUAGES CXX)

# C++ 표준 설정
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# MFC 사용 (Windows 전용)
if(WIN32)
    set(CMAKE_MFC_FLAG 2)
    add_definitions(-DUNICODE -D_UNICODE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /utf-8 /W3")
endif()

# 빌드 타입 설정
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

# 출력 디렉토리
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib/${CMAKE_BUILD_TYPE})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib/${CMAKE_BUILD_TYPE})

# ========================================
# 실제 사용되는 소스 파일 목록
# ========================================
set(SOURCES
    # 메인 애플리케이션
    ETrade_Order_Sample.cpp
    ETrade_Order_SampleDlg.cpp
    StdAfx.cpp
    
    # TR 코드 처리
    CSPAQ03700.cpp
    CSPAT00700.cpp
    CSPAT00800.cpp
    CSPBQ00200.cpp
    T0424.cpp
    T1101.cpp
    T1102.cpp
    
    # 로그인 및 주문 처리
    S_Login.cpp
    S_Order_Debt.cpp
    
    # 데이터베이스 연동
    MySQLCMD.cpp
    
    # 유틸리티
    GetList.cpp
    ListofLog.cpp
    Searching_List.cpp
    Setup.cpp
    DegreeList.cpp
    
    # Excel 연동
    XLAutomation.cpp
    XLEzAutomation.cpp
)

# 리소스 파일
set(RESOURCES
    ETrade_Order_Sample.rc
    res/ETrade_Order_Sample.rc2
)

# 헤더 파일 (명시적으로 포함하여 IntelliSense 지원)
set(HEADERS
    ETrade_Order_Sample.h
    ETrade_Order_SampleDlg.h
    StdAfx.h
    resource.h
    
    # TR 코드 헤더
    CSPAQ03700.h
    CSPAT00700.h
    CSPAT00800.h
    CSPBQ00200.h
    T0424.h
    T1101.h
    T1102.h
    
    # 기타 헤더
    S_Login.h
    S_Order_Debt.h
    MySQLCMD.h
    GetList.h
    ListofLog.h
    Searching_List.h
    Setup.h
    DegreeList.h
    IXingAPI.h
    QList.h
    XLAutomation.h
    XLEzAutomation.h
)

# 실행 파일 생성
add_executable(MMMaster WIN32 ${SOURCES} ${RESOURCES} ${HEADERS})

# 포함 디렉토리
target_include_directories(MMMaster PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/packet
)

# ========================================
# MySQL 라이브러리 찾기
# ========================================
find_path(MYSQL_INCLUDE_DIR mysql.h
    PATHS
    "C:/Program Files/MySQL/MySQL Connector C 8.0/include"
    "C:/Program Files/MySQL/mysql-8.0/include"
    "C:/MySQL/include"
    "C:/vcpkg/installed/x64-windows/include"
    ENV MYSQL_DIR
    PATH_SUFFIXES mysql
)

find_library(MYSQL_LIBRARY
    NAMES mysqlclient libmysql mysqlclient_r libmysql_r
    PATHS
    "C:/Program Files/MySQL/MySQL Connector C 8.0/lib"
    "C:/Program Files/MySQL/mysql-8.0/lib"
    "C:/MySQL/lib"
    "C:/vcpkg/installed/x64-windows/lib"
    ENV MYSQL_DIR
    PATH_SUFFIXES lib mysql
)

if(MYSQL_INCLUDE_DIR AND MYSQL_LIBRARY)
    target_include_directories(MMMaster PRIVATE ${MYSQL_INCLUDE_DIR})
    target_link_libraries(MMMaster ${MYSQL_LIBRARY})
    message(STATUS "MySQL found: ${MYSQL_INCLUDE_DIR}")
else()
    message(WARNING "MySQL not found. Database features may not work.")
    message(STATUS "To find MySQL, set MYSQL_DIR environment variable or install MySQL Connector/C")
endif()

# ========================================
# Windows 시스템 라이브러리
# ========================================
if(WIN32)
    target_link_libraries(MMMaster
        ws2_32      # 소켓
        wininet     # 인터넷
        ole32       # OLE
        oleaut32    # OLE Automation
    )
endif()

# ========================================
# 컴파일러 특정 옵션
# ========================================
if(MSVC)
    # 경고 설정
    target_compile_options(MMMaster PRIVATE
        /W3           # Warning level 3
        /WX-          # Warnings as errors: NO
        /permissive-  # Standards conformance
    )
    
    # 최적화 설정
    target_compile_options(MMMaster PRIVATE
        $<$<CONFIG:Release>:/O2 /DNDEBUG>
        $<$<CONFIG:Debug>:/Od /Zi /RTC1>
    )
    
    # 런타임 라이브러리 (MFC와 호환)
    set_property(TARGET MMMaster PROPERTY
        MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
    
    # PCH (Precompiled Header) 설정
    target_precompile_headers(MMMaster PRIVATE StdAfx.h)
endif()

# ========================================
# 빌드 후 작업: DLL 복사
# ========================================
if(WIN32)
    # XingAPI DLL 복사
    add_custom_command(TARGET MMMaster POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_CURRENT_SOURCE_DIR}/xingAPI.dll"
        $<TARGET_FILE_DIR:MMMaster>
        COMMENT "Copying xingAPI.dll"
    )
    
    # MySQL DLL 복사 (있는 경우)
    if(MYSQL_LIBRARY)
        get_filename_component(MYSQL_DLL_DIR ${MYSQL_LIBRARY} DIRECTORY)
        add_custom_command(TARGET MMMaster POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${MYSQL_DLL_DIR}/libmysql.dll"
            $<TARGET_FILE_DIR:MMMaster>
            COMMENT "Copying libmysql.dll"
        )
    endif()
endif()

# ========================================
# 디버깅 설정
# ========================================
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(MMMaster PRIVATE _DEBUG)
else()
    target_compile_definitions(MMMaster PRIVATE NDEBUG)
endif()

# ========================================
# 설치 규칙 (선택사항)
# ========================================
install(TARGETS MMMaster
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)

# ========================================
# 정보 출력
# ========================================
message(STATUS "========================================")
message(STATUS "MMMaster Build Configuration")
message(STATUS "========================================")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "MFC enabled: ${CMAKE_MFC_FLAG}")
message(STATUS "Output directory: ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
if(MYSQL_INCLUDE_DIR)
    message(STATUS "MySQL: ${MYSQL_INCLUDE_DIR}")
else()
    message(STATUS "MySQL: Not found")
endif()
message(STATUS "========================================")
