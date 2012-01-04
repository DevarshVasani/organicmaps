#include "../../testing/testing.hpp"

#include "../zip_reader.hpp"
#include "../file_writer.hpp"

#include "../../base/logging.hpp"
#include "../../base/macros.hpp"

static char const zipBytes[] = "PK\003\004\n\0\0\0\0\0\222\226\342>\302\032"
"x\372\005\0\0\0\005\0\0\0\b\0\034\0te"
"st.txtUT\t\0\003\303>\017N\017"
"?\017Nux\v\0\001\004\365\001\0\0\004P\0"
"\0\0Test\nPK\001\002\036\003\n\0\0"
"\0\0\0\222\226\342>\302\032x\372\005\0\0\0\005"
"\0\0\0\b\0\030\0\0\0\0\0\0\0\0\0\244"
"\201\0\0\0\0test.txtUT\005"
"\0\003\303>\017Nux\v\0\001\004\365\001\0\0"
"\004P\0\0\0PK\005\006\0\0\0\0\001\0\001"
"\0N\0\0\0G\0\0\0\0\0";

UNIT_TEST(ZipReaderSmoke)
{
  string const ZIPFILE = "smoke_test.zip";
  {
    FileWriter f(ZIPFILE);
    f.Write(zipBytes, ARRAY_SIZE(zipBytes) - 1);
  }

  bool noException = true;
  try
  {
    ZipFileReader r(ZIPFILE, "test.txt");
    string s;
    r.ReadAsString(s);
    TEST_EQUAL(s, "Test\n", ("Invalid zip file contents"));
  }
  catch (std::exception const & e)
  {
    noException = false;
    LOG(LERROR, (e.what()));
  }
  TEST(noException, ("Unhandled exception"));

  // invalid zip
  noException = true;
  try
  {
    ZipFileReader r("some_nonexisting_filename", "test.txt");
  }
  catch (std::exception const &)
  {
    noException = false;
  }
  TEST(!noException, ());

  // invalid file inside zip
  noException = true;
  try
  {
    ZipFileReader r(ZIPFILE, "test");
  }
  catch (std::exception const &)
  {
    noException = false;
  }
  TEST(!noException, ());

  FileWriter::DeleteFileX(ZIPFILE);
}

/// zip file with 3 files inside: 1.txt, 2.txt, 3.ttt
static char const zipBytes2[] = "\x50\x4b\x3\x4\xa\x0\x0\x0\x0\x0\x92\x6b\xf6\x3e\x53\xfc\x51\x67\x2\x0\x0"
"\x0\x2\x0\x0\x0\x5\x0\x1c\x0\x31\x2e\x74\x78\x74\x55\x54\x9\x0\x3\xd3\x50\x29\x4e\xd4\x50\x29\x4e\x75\x78"
"\xb\x0\x1\x4\xf5\x1\x0\x0\x4\x14\x0\x0\x0\x31\xa\x50\x4b\x3\x4\xa\x0\x0\x0\x0\x0\x95\x6b\xf6\x3e\x90\xaf"
"\x7c\x4c\x2\x0\x0\x0\x2\x0\x0\x0\x5\x0\x1c\x0\x32\x2e\x74\x78\x74\x55\x54\x9\x0\x3\xd9\x50\x29\x4e\xd9\x50"
"\x29\x4e\x75\x78\xb\x0\x1\x4\xf5\x1\x0\x0\x4\x14\x0\x0\x0\x32\xa\x50\x4b\x3\x4\xa\x0\x0\x0\x0\x0\x9c\x6b"
"\xf6\x3e\xd1\x9e\x67\x55\x2\x0\x0\x0\x2\x0\x0\x0\x5\x0\x1c\x0\x33\x2e\x74\x74\x74\x55\x54\x9\x0\x3\xe8\x50"
"\x29\x4e\xe9\x50\x29\x4e\x75\x78\xb\x0\x1\x4\xf5\x1\x0\x0\x4\x14\x0\x0\x0\x33\xa\x50\x4b\x1\x2\x1e\x3\xa"
"\x0\x0\x0\x0\x0\x92\x6b\xf6\x3e\x53\xfc\x51\x67\x2\x0\x0\x0\x2\x0\x0\x0\x5\x0\x18\x0\x0\x0\x0\x0\x1\x0\x0"
"\x0\xa4\x81\x0\x0\x0\x0\x31\x2e\x74\x78\x74\x55\x54\x5\x0\x3\xd3\x50\x29\x4e\x75\x78\xb\x0\x1\x4\xf5\x1\x0"
"\x0\x4\x14\x0\x0\x0\x50\x4b\x1\x2\x1e\x3\xa\x0\x0\x0\x0\x0\x95\x6b\xf6\x3e\x90\xaf\x7c\x4c\x2\x0\x0\x0\x2"
"\x0\x0\x0\x5\x0\x18\x0\x0\x0\x0\x0\x1\x0\x0\x0\xa4\x81\x41\x0\x0\x0\x32\x2e\x74\x78\x74\x55\x54\x5\x0\x3"
"\xd9\x50\x29\x4e\x75\x78\xb\x0\x1\x4\xf5\x1\x0\x0\x4\x14\x0\x0\x0\x50\x4b\x1\x2\x1e\x3\xa\x0\x0\x0\x0\x0"
"\x9c\x6b\xf6\x3e\xd1\x9e\x67\x55\x2\x0\x0\x0\x2\x0\x0\x0\x5\x0\x18\x0\x0\x0\x0\x0\x1\x0\x0\x0\xa4\x81\x82"
"\x0\x0\x0\x33\x2e\x74\x74\x74\x55\x54\x5\x0\x3\xe8\x50\x29\x4e\x75\x78\xb\x0\x1\x4\xf5\x1\x0\x0\x4\x14\x0"
"\x0\x0\x50\x4b\x5\x6\x0\x0\x0\x0\x3\x0\x3\x0\xe1\x0\x0\x0\xc3\x0\x0\x0\x0\x0";

static char const invalidZip[] = "1234567890asdqwetwezxvcbdhg322353tgfsd";

UNIT_TEST(ZipFilesList)
{
  string const ZIPFILE = "list_test.zip";
  {
    FileWriter f(ZIPFILE);
    f.Write(zipBytes2, ARRAY_SIZE(zipBytes2) - 1);
  }
  TEST(ZipFileReader::IsZip(ZIPFILE), ());
  string const ZIPFILE_INVALID = "invalid_test.zip";
  {
    FileWriter f(ZIPFILE_INVALID);
    f.Write(invalidZip, ARRAY_SIZE(invalidZip) - 1);
  }
  TEST(!ZipFileReader::IsZip(ZIPFILE_INVALID), ());

  try
  {
    vector<string> files = ZipFileReader::FilesList(ZIPFILE);
    TEST_EQUAL(files.size(), 3, ());
    TEST_EQUAL(files[0], "1.txt", ());
    TEST_EQUAL(files[1], "2.txt", ());
    TEST_EQUAL(files[2], "3.ttt", ());
  }
  catch (std::exception const & e)
  {
    TEST(false, ("Can't get list of files inside zip", e.what()));
  }

  try
  {
    vector<string> files = ZipFileReader::FilesList(ZIPFILE_INVALID);
    TEST(false, ("This test shouldn't be reached - exception should be thrown"));
  }
  catch (std::exception const &)
  {
  }

  FileWriter::DeleteFileX(ZIPFILE_INVALID);
  FileWriter::DeleteFileX(ZIPFILE);
}
