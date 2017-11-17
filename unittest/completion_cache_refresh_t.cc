/*
 * Copyright (c) 2017, Oracle and/or its affiliates. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; version 2 of the
 * License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301  USA
 */

#include "unittest/gtest_clean.h"
#include "unittest/test_utils/mocks/gmock_clean.h"

#include "mysqlsh/cmdline_shell.h"
#include "unittest/test_utils.h"

namespace mysqlsh {

// schema name cache
// full object name cache

// no session
// session no schema
// session schema

// change schema

// sql mode
// scripting mode

class Completion_cache_refresh : public Shell_core_test_wrapper {
 public:
  void reset_shell() override {
    _interactive_shell.reset(new Command_line_shell(
        _opts, std::unique_ptr<shcore::Interpreter_delegate>{
                  new shcore::Interpreter_delegate(output_handler.deleg)}));
    set_defaults();
  }

  void reset_shell(const std::string &uri, shcore::IShell_core::Mode mode) {
    wipe_all();
    _options->interactive = true;
    _options->uri = uri;
    _options->initial_mode = mode;
    reset_shell();
    if (!uri.empty()) {
      _interactive_shell->connect(_options->connection_options(), false);
    }
  }
};

// ---------------------------------------------------------------------------
// Tests to check if automatic refresh of schema and table name cache happens at
// the expected times

// FR10
TEST_F(Completion_cache_refresh, startup) {
  _options->db_name_cache = true;
  _options->devapi_schema_object_handles = true;

  // No connectin, no refresh
  reset_shell("", shcore::IShell_core::Mode::JavaScript);
  MY_EXPECT_STDOUT_NOT_CONTAINS("Creating a session to");
  MY_EXPECT_STDOUT_NOT_CONTAINS("Fetching schema names");
  MY_EXPECT_STDOUT_NOT_CONTAINS("Fetching table and column names from");

  reset_shell("", shcore::IShell_core::Mode::Python);
  MY_EXPECT_STDOUT_NOT_CONTAINS("Creating a session to");
  MY_EXPECT_STDOUT_NOT_CONTAINS("Fetching schema names");
  MY_EXPECT_STDOUT_NOT_CONTAINS("Fetching table and column names from");

  reset_shell("", shcore::IShell_core::Mode::SQL);
  MY_EXPECT_STDOUT_NOT_CONTAINS("Creating a session to");
  MY_EXPECT_STDOUT_NOT_CONTAINS("Fetching schema names");
  MY_EXPECT_STDOUT_NOT_CONTAINS("Fetching table and column names from");

  // Classic
  // Connection without default schema, refresh schemas only
  reset_shell(_mysql_uri, shcore::IShell_core::Mode::JavaScript);
  MY_EXPECT_STDOUT_CONTAINS("Creating a session to");
  MY_EXPECT_STDOUT_CONTAINS("Fetching schema names");
  MY_EXPECT_STDOUT_NOT_CONTAINS("Fetching table and column names from");

  reset_shell(_mysql_uri, shcore::IShell_core::Mode::Python);
  MY_EXPECT_STDOUT_CONTAINS("Creating a session to");
  MY_EXPECT_STDOUT_CONTAINS("Fetching schema names");
  MY_EXPECT_STDOUT_NOT_CONTAINS("Fetching table and column names from");

  reset_shell(_mysql_uri, shcore::IShell_core::Mode::SQL);
  MY_EXPECT_STDOUT_CONTAINS("Creating a session to");
  MY_EXPECT_STDOUT_CONTAINS("Fetching schema names");
  MY_EXPECT_STDOUT_NOT_CONTAINS("Fetching table and column names from");

  // Connection with default schema, refresh schemas and objects
  reset_shell(_mysql_uri + "/mysql", shcore::IShell_core::Mode::JavaScript);
  MY_EXPECT_STDOUT_CONTAINS("Creating a session to");
  MY_EXPECT_STDOUT_CONTAINS("Fetching schema names");
  MY_EXPECT_STDOUT_NOT_CONTAINS("Fetching table and column names from `mysql`");

  reset_shell(_mysql_uri + "/mysql", shcore::IShell_core::Mode::Python);
  MY_EXPECT_STDOUT_CONTAINS("Creating a session to");
  MY_EXPECT_STDOUT_CONTAINS("Fetching schema names");
  MY_EXPECT_STDOUT_NOT_CONTAINS("Fetching table and column names from `mysql`");

  reset_shell(_mysql_uri + "/mysql", shcore::IShell_core::Mode::SQL);
  MY_EXPECT_STDOUT_CONTAINS("Creating a session to");
  MY_EXPECT_STDOUT_CONTAINS("Fetching schema names");
  MY_EXPECT_STDOUT_CONTAINS("Fetching table and column names from `mysql`");

  // X proto
  // Connection without default schema, refresh schemas only
  reset_shell(_uri, shcore::IShell_core::Mode::JavaScript);
  MY_EXPECT_STDOUT_CONTAINS("Creating a session to");
  MY_EXPECT_STDOUT_CONTAINS("Fetching schema names");
  MY_EXPECT_STDOUT_NOT_CONTAINS("Fetching table and column names from");

  reset_shell(_uri, shcore::IShell_core::Mode::Python);
  MY_EXPECT_STDOUT_CONTAINS("Creating a session to");
  MY_EXPECT_STDOUT_CONTAINS("Fetching schema names");
  MY_EXPECT_STDOUT_NOT_CONTAINS("Fetching table and column names from");

  reset_shell(_uri, shcore::IShell_core::Mode::SQL);
  MY_EXPECT_STDOUT_CONTAINS("Creating a session to");
  MY_EXPECT_STDOUT_CONTAINS("Fetching schema names");
  MY_EXPECT_STDOUT_NOT_CONTAINS("Fetching table and column names from");

  // Connection with default schema, refresh schemas and objects
  reset_shell(_uri + "/mysql", shcore::IShell_core::Mode::JavaScript);
  MY_EXPECT_STDOUT_CONTAINS("Creating a session to");
  MY_EXPECT_STDOUT_CONTAINS("Fetching schema names");
  MY_EXPECT_STDOUT_NOT_CONTAINS("Fetching table and column names from `mysql`");
  wipe_all();
  // Check schema object cache in DevAPI
  execute("dir(db)");
  MY_EXPECT_STDOUT_CONTAINS("\"user\"");
  MY_EXPECT_STDOUT_CONTAINS("\"help_category\"");

  reset_shell(_uri + "/mysql", shcore::IShell_core::Mode::Python);
  MY_EXPECT_STDOUT_CONTAINS("Creating a session to");
  MY_EXPECT_STDOUT_CONTAINS("Fetching schema names");
  MY_EXPECT_STDOUT_NOT_CONTAINS("Fetching table and column names from `mysql`");
  // Check schema object cache in DevAPI
  execute("dir(db)");
  MY_EXPECT_STDOUT_CONTAINS("\"user\"");
  MY_EXPECT_STDOUT_CONTAINS("\"help_category\"");

  reset_shell(_uri + "/mysql", shcore::IShell_core::Mode::SQL);
  MY_EXPECT_STDOUT_CONTAINS("Creating a session to");
  MY_EXPECT_STDOUT_CONTAINS("Fetching schema names");
  MY_EXPECT_STDOUT_CONTAINS("Fetching table and column names from `mysql`");
}

// FR10
TEST_F(Completion_cache_refresh, connect_interactive) {
  _options->db_name_cache = true;
  _options->devapi_schema_object_handles = true;

  // Start with no connection
  reset_shell("", shcore::IShell_core::Mode::JavaScript);
  MY_EXPECT_STDOUT_NOT_CONTAINS("Creating a session to");
  MY_EXPECT_STDOUT_NOT_CONTAINS("Fetching schema names");
  MY_EXPECT_STDOUT_NOT_CONTAINS("Fetching table and column names from");
  // Connect interactively
  wipe_out();
  execute("\\connect " + _mysql_uri);
  MY_EXPECT_STDOUT_CONTAINS("Creating a session to");
  MY_EXPECT_STDOUT_CONTAINS("Fetching schema names");
  MY_EXPECT_STDOUT_NOT_CONTAINS("Fetching table and column names from");
  wipe_out();
  execute("\\connect " + _uri);
  MY_EXPECT_STDOUT_CONTAINS("Creating a session to");
  MY_EXPECT_STDOUT_CONTAINS("Fetching schema names");
  MY_EXPECT_STDOUT_NOT_CONTAINS("Fetching table and column names from");
  wipe_out();
  execute("\\connect " + _uri + "/mysql");
  MY_EXPECT_STDOUT_CONTAINS("Creating a session to");
  MY_EXPECT_STDOUT_CONTAINS("Fetching schema names");
  MY_EXPECT_STDOUT_NOT_CONTAINS("Fetching table and column names from `mysql`");
  wipe_out();
  execute("shell.connect('" + _uri + "/sys')");
  MY_EXPECT_STDOUT_CONTAINS("Creating a session to");
  MY_EXPECT_STDOUT_CONTAINS("Fetching schema names");
  MY_EXPECT_STDOUT_NOT_CONTAINS("Fetching table and column names from `sys`");

  reset_shell("", shcore::IShell_core::Mode::Python);
  MY_EXPECT_STDOUT_NOT_CONTAINS("Creating a session to");
  MY_EXPECT_STDOUT_NOT_CONTAINS("Fetching schema names");
  MY_EXPECT_STDOUT_NOT_CONTAINS("Fetching table and column names from");
  wipe_out();
  execute("\\connect " + _mysql_uri);
  MY_EXPECT_STDOUT_CONTAINS("Creating a session to");
  MY_EXPECT_STDOUT_CONTAINS("Fetching schema names");
  MY_EXPECT_STDOUT_NOT_CONTAINS("Fetching table and column names from");
  wipe_out();
  execute("\\connect " + _uri);
  MY_EXPECT_STDOUT_CONTAINS("Creating a session to");
  MY_EXPECT_STDOUT_CONTAINS("Fetching schema names");
  MY_EXPECT_STDOUT_NOT_CONTAINS("Fetching table and column names from");
  wipe_out();
  execute("\\connect " + _uri + "/mysql");
  MY_EXPECT_STDOUT_CONTAINS("Creating a session to");
  MY_EXPECT_STDOUT_CONTAINS("Fetching schema names");
  MY_EXPECT_STDOUT_NOT_CONTAINS("Fetching table and column names from `mysql`");
  wipe_out();
  execute("shell.connect('" + _uri + "/sys')");
  MY_EXPECT_STDOUT_CONTAINS("Creating a session to");
  MY_EXPECT_STDOUT_CONTAINS("Fetching schema names");
  MY_EXPECT_STDOUT_NOT_CONTAINS("Fetching table and column names from `sys`");

  reset_shell("", shcore::IShell_core::Mode::SQL);
  MY_EXPECT_STDOUT_NOT_CONTAINS("Creating a session to");
  MY_EXPECT_STDOUT_NOT_CONTAINS("Fetching schema names");
  MY_EXPECT_STDOUT_NOT_CONTAINS("Fetching table and column names from");
  wipe_out();
  execute("\\connect " + _mysql_uri);
  MY_EXPECT_STDOUT_CONTAINS("Creating a session to");
  MY_EXPECT_STDOUT_CONTAINS("Fetching schema names");
  MY_EXPECT_STDOUT_NOT_CONTAINS("Fetching table and column names from");
  wipe_out();
  execute("\\connect " + _uri);
  MY_EXPECT_STDOUT_CONTAINS("Creating a session to");
  MY_EXPECT_STDOUT_CONTAINS("Fetching schema names");
  MY_EXPECT_STDOUT_NOT_CONTAINS("Fetching table and column names from");
  wipe_out();
  execute("\\connect " + _uri + "/mysql");
  MY_EXPECT_STDOUT_CONTAINS("Creating a session to");
  MY_EXPECT_STDOUT_CONTAINS("Fetching schema names");
  MY_EXPECT_STDOUT_CONTAINS("Fetching table and column names from `mysql`");
}

// FR10
TEST_F(Completion_cache_refresh, switch_to_sql) {
  _options->db_name_cache = true;
  _options->devapi_schema_object_handles = true;

  reset_shell("", shcore::IShell_core::Mode::JavaScript);
  MY_EXPECT_STDOUT_NOT_CONTAINS("Creating a session to");
  MY_EXPECT_STDOUT_NOT_CONTAINS("Fetching schema names");
  MY_EXPECT_STDOUT_NOT_CONTAINS("Fetching table and column names from");
  wipe_all();
  execute("\\sql");
  MY_EXPECT_STDOUT_NOT_CONTAINS("Fetching schema names");
  MY_EXPECT_STDOUT_NOT_CONTAINS("Fetching table and column names from");

  reset_shell(_mysql_uri, shcore::IShell_core::Mode::JavaScript);
  MY_EXPECT_STDOUT_CONTAINS("Creating a session to");
  MY_EXPECT_STDOUT_CONTAINS("Fetching schema names");
  MY_EXPECT_STDOUT_NOT_CONTAINS("Fetching table and column names from");
  wipe_all();
  execute("\\sql");
  MY_EXPECT_STDOUT_NOT_CONTAINS("Fetching schema names");
  MY_EXPECT_STDOUT_NOT_CONTAINS("Fetching table and column names from");

  reset_shell(_mysql_uri + "/mysql", shcore::IShell_core::Mode::JavaScript);
  MY_EXPECT_STDOUT_CONTAINS("Creating a session to");
  MY_EXPECT_STDOUT_CONTAINS("Fetching schema names");
  MY_EXPECT_STDOUT_NOT_CONTAINS("Fetching table and column names from");
  wipe_all();
  execute("\\sql");
  MY_EXPECT_STDOUT_NOT_CONTAINS("Fetching schema names");
  MY_EXPECT_STDOUT_CONTAINS("Fetching table and column names from `mysql`");

  reset_shell(_uri, shcore::IShell_core::Mode::JavaScript);
  MY_EXPECT_STDOUT_CONTAINS("Creating a session to");
  MY_EXPECT_STDOUT_CONTAINS("Fetching schema names");
  MY_EXPECT_STDOUT_NOT_CONTAINS("Fetching table and column names from");
  wipe_all();
  execute("\\sql");
  MY_EXPECT_STDOUT_NOT_CONTAINS("Fetching schema names");
  MY_EXPECT_STDOUT_NOT_CONTAINS("Fetching table and column names from");

  reset_shell(_uri + "/mysql", shcore::IShell_core::Mode::JavaScript);
  MY_EXPECT_STDOUT_CONTAINS("Creating a session to");
  MY_EXPECT_STDOUT_CONTAINS("Fetching schema names");
  MY_EXPECT_STDOUT_NOT_CONTAINS("Fetching table and column names from");
  wipe_all();
  execute("\\sql");
  MY_EXPECT_STDOUT_NOT_CONTAINS("Fetching schema names");
  MY_EXPECT_STDOUT_CONTAINS("Fetching table and column names from `mysql`");
}

// FR10
TEST_F(Completion_cache_refresh, switch_schema_js) {
  _options->db_name_cache = true;
  _options->devapi_schema_object_handles = true;

  reset_shell(_mysql_uri, shcore::IShell_core::Mode::JavaScript);
  MY_EXPECT_STDOUT_CONTAINS("Creating a session to");
  MY_EXPECT_STDOUT_CONTAINS("Fetching schema names");
  MY_EXPECT_STDOUT_NOT_CONTAINS("Fetching table and column names from");
  wipe_all();
  execute("\\use mysql");
  MY_EXPECT_STDOUT_NOT_CONTAINS("Fetching schema names");
  MY_EXPECT_STDOUT_NOT_CONTAINS("Fetching table and column names");
  execute("\\use information_schema");
  MY_EXPECT_STDOUT_NOT_CONTAINS("Fetching schema names");
  MY_EXPECT_STDOUT_NOT_CONTAINS("Fetching table and column names");
}

// FR10
TEST_F(Completion_cache_refresh, switch_schema_sql) {
  _options->db_name_cache = true;
  _options->devapi_schema_object_handles = true;

  reset_shell(_mysql_uri, shcore::IShell_core::Mode::SQL);
  MY_EXPECT_STDOUT_CONTAINS("Creating a session to");
  MY_EXPECT_STDOUT_CONTAINS("Fetching schema names");
  MY_EXPECT_STDOUT_NOT_CONTAINS("Fetching table and column names from");
  wipe_all();
  execute("\\use mysql");
  MY_EXPECT_STDOUT_NOT_CONTAINS("Fetching schema names");
  MY_EXPECT_STDOUT_CONTAINS("Fetching table and column names from `mysql`");
  execute("\\use information_schema");
  MY_EXPECT_STDOUT_NOT_CONTAINS("Fetching schema names");
  MY_EXPECT_STDOUT_CONTAINS(
      "Fetching table and column names from `information_schema`");

  reset_shell(_mysql_uri, shcore::IShell_core::Mode::JavaScript);
  MY_EXPECT_STDOUT_CONTAINS("Creating a session to");
  MY_EXPECT_STDOUT_CONTAINS("Fetching schema names");
  MY_EXPECT_STDOUT_NOT_CONTAINS("Fetching table and column names from");
  wipe_all();
  execute("\\use mysql");
  MY_EXPECT_STDOUT_NOT_CONTAINS("Fetching schema names");
  MY_EXPECT_STDOUT_NOT_CONTAINS("Fetching table and column names from `mysql`");
  wipe_all();
  execute("\\sql");
  MY_EXPECT_STDOUT_NOT_CONTAINS("Fetching schema names");
  MY_EXPECT_STDOUT_CONTAINS("Fetching table and column names from `mysql`");
  wipe_all();
  execute("\\js");
  MY_EXPECT_STDOUT_NOT_CONTAINS("Fetching schema names");
  MY_EXPECT_STDOUT_NOT_CONTAINS("Fetching table and column names");
  wipe_all();
  execute("\\sql");
  MY_EXPECT_STDOUT_NOT_CONTAINS("Fetching schema names");
  MY_EXPECT_STDOUT_NOT_CONTAINS("Fetching table and column names");
}

// FR11
TEST_F(Completion_cache_refresh, rehash) {
  _options->db_name_cache = true;
  _options->devapi_schema_object_handles = true;

  reset_shell("", shcore::IShell_core::Mode::SQL);
  MY_EXPECT_STDOUT_NOT_CONTAINS("Fetching schema names");
  MY_EXPECT_STDOUT_NOT_CONTAINS("Fetching table and column names from");
  wipe_all();
  execute("\\rehash");
  MY_EXPECT_STDOUT_NOT_CONTAINS("Fetching schema names");
  MY_EXPECT_STDOUT_NOT_CONTAINS("Fetching table and column names from");
  MY_EXPECT_STDOUT_CONTAINS("Not connected.");

  reset_shell(_uri, shcore::IShell_core::Mode::SQL);
  MY_EXPECT_STDOUT_CONTAINS("Fetching schema names");
  MY_EXPECT_STDOUT_NOT_CONTAINS("Fetching table and column names from");
  wipe_all();
  execute("\\rehash");
  MY_EXPECT_STDOUT_CONTAINS("Fetching schema names");
  MY_EXPECT_STDOUT_NOT_CONTAINS("Fetching table and column names from");

  reset_shell(_uri + "/mysql", shcore::IShell_core::Mode::SQL);
  MY_EXPECT_STDOUT_CONTAINS("Fetching schema names");
  MY_EXPECT_STDOUT_CONTAINS("Fetching table and column names from `mysql`");
  wipe_all();
  execute("\\rehash");
  MY_EXPECT_STDOUT_CONTAINS("Fetching schema names");
  MY_EXPECT_STDOUT_CONTAINS("Fetching table and column names from `mysql`");
  wipe_all();
  execute("\\rehash");
  MY_EXPECT_STDOUT_CONTAINS("Fetching schema names");
  MY_EXPECT_STDOUT_CONTAINS("Fetching table and column names from `mysql`");
}

// FR11
TEST_F(Completion_cache_refresh, rehash_db) {
  _options->db_name_cache = true;
  _options->devapi_schema_object_handles = true;

  // Check that rehash also updates the object cache in db
  reset_shell(_uri, shcore::IShell_core::Mode::JavaScript);
  execute("session.sql('drop schema if exists dropme').execute();");
  execute("session.sql('create schema dropme').execute();");
  execute("session.sql('create table dropme.tbl (a int);').execute()");
  wipe_all();
  execute("db.dropme");
  MY_EXPECT_STDOUT_NOT_CONTAINS("<Table:dropme>");
  execute("\\use dropme");
  wipe_all();
  execute("db.tbl");
  MY_EXPECT_STDOUT_CONTAINS("<Table:tbl>");
  wipe_all();
  execute("db.dropme");
  MY_EXPECT_STDOUT_NOT_CONTAINS("<Table:dropme>");
  execute("session.sql('create table dropme.dropme (a int);').execute()");
  wipe_all();
  execute("db.dropme");
  MY_EXPECT_STDOUT_NOT_CONTAINS("<Table:dropme>");
  execute("\\rehash");
  execute("db.dropme");
  MY_EXPECT_STDOUT_CONTAINS("<Table:dropme>");

  _options->devapi_schema_object_handles = true;
  reset_shell(_uri + "/mysql", shcore::IShell_core::Mode::JavaScript);
  wipe_all();
  execute("db.user");
  MY_EXPECT_STDOUT_CONTAINS("<Table:user>");
  execute("\\rehash");
  wipe_all();
  execute("db.user");
  MY_EXPECT_STDOUT_CONTAINS("<Table:user>");
  wipe_all();
  execute("dir(db)");
  MY_EXPECT_STDOUT_CONTAINS("help_category");

  _options->devapi_schema_object_handles = false;
  reset_shell(_uri + "/mysql", shcore::IShell_core::Mode::JavaScript);
  wipe_all();
  execute("db.user");
  MY_EXPECT_STDOUT_NOT_CONTAINS("<Table:user>");
  execute("\\rehash");
  wipe_all();
  execute("db.user");
  MY_EXPECT_STDOUT_NOT_CONTAINS("<Table:user>");
  wipe_all();
  execute("dir(db)");
  MY_EXPECT_STDOUT_NOT_CONTAINS("help_category");
  execute("shell.options['devapi.dbObjectHandles']=true;");
  execute("\\rehash");
  wipe_all();
  execute("db.user");
  MY_EXPECT_STDOUT_CONTAINS("<Table:user>");
  wipe_all();
  execute("dir(db)");
  MY_EXPECT_STDOUT_CONTAINS("help_category");
  execute("shell.options['devapi.dbObjectHandles']=false;");
  wipe_all();
  execute("db.user");
  MY_EXPECT_STDOUT_NOT_CONTAINS("<Table:user>");
  wipe_all();
  execute("dir(db)");
  MY_EXPECT_STDOUT_NOT_CONTAINS("help_category");

  execute("session.sql('drop schema if exists dropme').execute();");
}

// FR10
TEST_F(Completion_cache_refresh, options_db_object_handles) {
  _options->db_name_cache = true;
  _options->devapi_schema_object_handles = true;
  reset_shell(_uri + "/mysql", shcore::IShell_core::Mode::JavaScript);
  execute("shell.options['devapi.dbObjectHandles']");
  MY_EXPECT_STDOUT_CONTAINS("true");

  _options->db_name_cache = true;
  _options->devapi_schema_object_handles = false;
  reset_shell(_uri + "/mysql", shcore::IShell_core::Mode::JavaScript);
  wipe_all();
  execute("db.user");
  MY_EXPECT_STDOUT_NOT_CONTAINS("<Table:user>");
  execute("\\rehash");  // rehash does not populate db if dbObjectHandles=false
  wipe_all();
  execute("db.user");
  MY_EXPECT_STDOUT_NOT_CONTAINS("<Table:user>");
  wipe_all();
  execute("dir(db)");
  MY_EXPECT_STDOUT_NOT_CONTAINS("help_category");
  wipe_all();
  execute("shell.options['devapi.dbObjectHandles']");
  MY_EXPECT_STDOUT_CONTAINS("false");
  execute("shell.options['devapi.dbObjectHandles']=true;");
  execute("\\rehash");  // rehash required after enabling db handles
  wipe_all();
  execute("db.user");
  MY_EXPECT_STDOUT_CONTAINS("<Table:user>");
  wipe_all();
  execute("dir(db)");
  MY_EXPECT_STDOUT_CONTAINS("help_category");
  execute("shell.options['devapi.dbObjectHandles']=false;");
  wipe_all();
  execute("db.user");
  MY_EXPECT_STDOUT_NOT_CONTAINS("<Table:user>");
  wipe_all();
  execute("dir(db)");
  MY_EXPECT_STDOUT_NOT_CONTAINS("help_category");
}

// FR11
TEST_F(Completion_cache_refresh, options_db_name_cache) {
  _options->db_name_cache = true;
  _options->devapi_schema_object_handles = true;
  reset_shell(_uri + "/mysql", shcore::IShell_core::Mode::JavaScript);
  execute("shell.options['autocomplete.nameCache']");
  MY_EXPECT_STDOUT_CONTAINS("true");

  _options->db_name_cache = false;
  _options->devapi_schema_object_handles = true;
  reset_shell(_uri + "/mysql", shcore::IShell_core::Mode::SQL);
  MY_EXPECT_STDOUT_NOT_CONTAINS("Fetching table and column");
  wipe_all();
  execute("\\rehash");
  MY_EXPECT_STDOUT_CONTAINS("Fetching table and column");
}

}  // namespace mysqlsh