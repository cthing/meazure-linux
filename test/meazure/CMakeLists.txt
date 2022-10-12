set(MOCK_SOURCES
    mocks/MockScreenInfoProvider.h
    )

# Builds and adds the specified test runner program to the list of unit tests to run.
#
# runner - Name of the test runner source file without the .cpp extension
# srcdir - Directory containing the test source file
# ...    - Additional source files required to build the test runner
#
macro(ADD_MEAZURE_TEST runner srcdir)
    add_executable(${runner} ${srcdir}/${runner}.cpp ${MOCK_SOURCES} testing/TestHelpers.h)
    add_dependencies(${runner} libmeazure)
    target_include_directories(${runner} PRIVATE
                               $<TARGET_PROPERTY:meazure,INCLUDE_DIRECTORIES>
                               ${PROJECT_SOURCE_DIR}
                               )
    target_link_directories(${runner} PRIVATE $<TARGET_PROPERTY:meazure,LINK_DIRECTORIES>)
    target_link_libraries(${runner} PRIVATE
                          $<TARGET_PROPERTY:meazure,LINK_LIBRARIES>
                          Qt6::Test
                          )
    add_test(NAME ${runner} COMMAND ${runner})
endmacro()

ADD_MEAZURE_TEST(ColorsTest graphics)
ADD_MEAZURE_TEST(FileProfileTest profile)
ADD_MEAZURE_TEST(GeometryTest utils)
ADD_MEAZURE_TEST(MathUtilsTest utils)
ADD_MEAZURE_TEST(SettingsProfileTest profile)
ADD_MEAZURE_TEST(StringUtilsTest utils)
ADD_MEAZURE_TEST(UnitsTest units)
ADD_MEAZURE_TEST(XMLParserTest xml)
ADD_MEAZURE_TEST(XMLWriterTest xml)
