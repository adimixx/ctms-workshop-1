cmake_minimum_required(VERSION 3.16)
project(Workshop1)
set(CMAKE_CXX_STANDARD 14)

include(${CMAKE_CURRENT_LIST_DIR}/conan/conanbuildinfo.cmake)
conan_basic_setup()

include_directories(Database)
include_directories(Dependency)
include_directories(RouteManagement)
include_directories(VesselManagement)
include_directories(PackageManagement)
include_directories(TicketManagement)

set(ROUTE_MANAGEMENT_FILES RouteManagement/RouteManagement.h RouteManagement/Route.cpp RouteManagement/Route_Add.cpp RouteManagement/Route_Delete.cpp RouteManagement/Route_List.cpp)
set(VESSEL_MANAGEMENT_FILES VesselManagement/VesselManagement.h VesselManagement/Vesssel.cpp VesselManagement/Vessel_List.cpp VesselManagement/Vessel_Delete.cpp VesselManagement/Vessel_AddEdit.cpp VesselManagement/Deck.cpp)
set(PACKAGE_MANAGEMENT_FILES PackageManagement/PackageManagement.h PackageManagement/Package.cpp PackageManagement/Package_AddEdit.cpp PackageManagement/Package_List.cpp TicketManagement/TicketManagement.h TicketManagement/Ticket.cpp TicketManagement/TicketCounter_Add.cpp)

add_executable(Workshop1 ${PACKAGE_MANAGEMENT_FILES} ${ROUTE_MANAGEMENT_FILES} ${VESSEL_MANAGEMENT_FILES} main.cpp Core.h Dependency/TextTable.h Dependency/cpplinq.hpp Dependency/Database.cpp Dependency/Database.h Dependency/md5.cpp Dependency/md5.h Dependency/Input.cpp Dependency/Input.h Database/User.h Database/CtmsDB.cpp Database/CtmsDB.h Dependency/Dependency.h Database/User_Role.h Database/Deck.h Database/Package.h Database/Package_Route.h Database/Receipt.h Database/Role.h Database/Route.h Database/Ticket.h Database/Ticket_Type.h Database/Value_Type.h Database/Vessel.h)
target_link_libraries(${PROJECT_NAME} ${CONAN_LIBS})


#list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")
#include(FindPkgConfig)
#find_package(Boost)
#INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})
#TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${Boost_LIBRARIES})
#
#find_package(MySQL REQUIRED)
#TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${MYSQL_CLIENT_LIBS})
