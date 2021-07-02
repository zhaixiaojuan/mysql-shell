#@ {has_oci_environment('OS')}
# Test dump and load into/from OCI ObjectStorage

#@<> INCLUDE oci_utils.inc

#@<> INCLUDE dump_utils.inc

#@<> Setup

import oci
import os
import json
import datetime

oci_config_file=os.path.join(OCI_CONFIG_HOME, "config")

testutil.deploy_sandbox(__mysql_sandbox_port1, "root")

session1=mysql.get_session(__sandbox_uri1)

session1.run_sql("create schema sample")
session1.run_sql("create table sample.data(id int, name varchar(20))")
session1.run_sql("insert into sample.data values (10, 'John Doe')")
session1.run_sql("create table sample.data_copy(id int, name varchar(20))")
session1.run_sql("insert into sample.data_copy values (5, 'Jane Doe')")
session1.close()

# ------------
# Defaults
RFC3339 = True
shell.connect(__sandbox_uri1)

# Prepare dump on bucket location (using prefix)
prepare_empty_bucket(OS_BUCKET_NAME, OS_NAMESPACE)

PREPARE_PAR_IS_SECRET_TEST()
util.dump_instance("", {"osBucketName":OS_BUCKET_NAME, "osNamespace": OS_NAMESPACE, "ociConfigFile":oci_config_file})
EXPECT_PAR_IS_SECRET()

testutil.deploy_sandbox(__mysql_sandbox_port2, "root", {"local_infile":1})
shell.connect(__sandbox_uri2)

#@<> WL14645-TSFR_1_1 - Successfully load dump with bucket AnyObjectRead PAR
all_read_par=create_par(OS_NAMESPACE, OS_BUCKET_NAME, "AnyObjectRead", "all-read-par", today_plus_days(1, RFC3339), None, "ListObjects")

remove_local_progress_file()

PREPARE_PAR_IS_SECRET_TEST()
EXPECT_NO_THROWS(lambda: util.load_dump(all_read_par, {"progressFile": local_progress_file}), "load_dump() using local progress file")
EXPECT_PAR_IS_SECRET()

EXPECT_STDOUT_CONTAINS("2 tables in 1 schemas were loaded")
validate_load_progress(local_progress_file)
session.run_sql("drop schema if exists sample")


#@<> WL14645-TSFR_1_3 - Successfully load dump with bucket AnyObjectReadWrite PAR
all_read_write_par=create_par(OS_NAMESPACE, OS_BUCKET_NAME, "AnyObjectReadWrite", "all-read-write-par", today_plus_days(1, RFC3339), None, "ListObjects")

remove_local_progress_file()

PREPARE_PAR_IS_SECRET_TEST()
EXPECT_NO_THROWS(lambda: util.load_dump(all_read_write_par, {"progressFile": local_progress_file}), "load_dump() using local progress file")
EXPECT_PAR_IS_SECRET()

EXPECT_STDOUT_CONTAINS("2 tables in 1 schemas were loaded")
validate_load_progress(local_progress_file)
session.run_sql("drop schema if exists sample")

#@<> WL14645-TSFR_1_5 - Failed load dump with bucket AnyObjectWrite PAR
all_write_par=create_par(OS_NAMESPACE, OS_BUCKET_NAME, "AnyObjectWrite", "all-read-par", today_plus_days(1, RFC3339))

remove_local_progress_file()

PREPARE_PAR_IS_SECRET_TEST()
EXPECT_THROWS(lambda: util.load_dump(all_write_par, {"progressFile": local_progress_file}), "Util.load_dump: Not Found")
EXPECT_PAR_IS_SECRET()

#@<> WL14645-TSFR_1_7 - Failed load dump with bucket AnyObjectRead PAR without ListObjects
all_read_par_no_list=create_par(OS_NAMESPACE, OS_BUCKET_NAME, "AnyObjectRead", "all-read-par", today_plus_days(1, RFC3339))

remove_local_progress_file()

PREPARE_PAR_IS_SECRET_TEST()
EXPECT_THROWS(lambda: util.load_dump(all_read_par_no_list, {"progressFile": local_progress_file}),
  f"Util.load_dump: Either the bucket named '{OS_BUCKET_NAME}' does not exist in the namespace '{OS_NAMESPACE}' or you are not authorized to access it")
EXPECT_PAR_IS_SECRET()

#@<> WL14645-TSFR_1_9 - Failed load dump with bucket AnyObjectWrite PAR and ListObjects
all_write_par_and_list=create_par(OS_NAMESPACE, OS_BUCKET_NAME, "AnyObjectWrite", "all-read-par", today_plus_days(1, RFC3339), None, "ListObjects")

remove_local_progress_file()

PREPARE_PAR_IS_SECRET_TEST()
EXPECT_THROWS(lambda: util.load_dump(all_write_par_and_list, {"progressFile": local_progress_file}), "Util.load_dump: Not Found")
EXPECT_PAR_IS_SECRET()

#@<> WL14645-TSFR_1_11 - Failed load dump, missing progress file
all_write_par_and_list=create_par(OS_NAMESPACE, OS_BUCKET_NAME, "AnyObjectRead", "all-read-par", today_plus_days(1, RFC3339), None, "ListObjects")

PREPARE_PAR_IS_SECRET_TEST()
EXPECT_THROWS(lambda: util.load_dump(all_write_par_and_list),
  "Util.load_dump: When using a PAR to load a dump, the progressFile option must be defined")
EXPECT_PAR_IS_SECRET()

PREPARE_PAR_IS_SECRET_TEST()
EXPECT_THROWS(lambda: util.load_dump(all_write_par_and_list, {"progressFile": "http://whatever-file_starting-with-http"}),
  "Util.load_dump: When using a prefix PAR to load a dump, the progressFile option must be a local file")
EXPECT_PAR_IS_SECRET()

#@<> WL14645-TSFR_1_13 - Failed load dump, missing dump
all_write_par_and_list_no_dump=create_par(OS_NAMESPACE, OS_BUCKET_NAME, "AnyObjectRead", "all-read-par", today_plus_days(1, RFC3339), "random-folder/", "ListObjects")

remove_local_progress_file()

PREPARE_PAR_IS_SECRET_TEST()
EXPECT_THROWS(lambda: util.load_dump(all_write_par_and_list_no_dump, {"progressFile": local_progress_file}),
  "Util.load_dump: Not Found")
EXPECT_PAR_IS_SECRET()

#@<> BUG#33122234 - setup
tested_schema = "test_schema"
tested_table = "test_table_"
# each table has 4 files associated with it, this should add up to more than 1000 files
n_tables = 250

# create schema with lots of tables
shell.connect(__sandbox_uri1)
session.run_sql("CREATE SCHEMA !;", [ tested_schema ])
session.run_sql("CREATE TABLE !.! (id INT PRIMARY KEY);", [ tested_schema, tested_table ])
session.run_sql("INSERT INTO !.! (id) VALUES (1234);", [ tested_schema, tested_table ])
session.run_sql("ANALYZE TABLE !.!;", [ tested_schema, tested_table ])

for i in range(n_tables):
    table_name = f"{tested_table}{i}"
    session.run_sql("CREATE TABLE !.! (PRIMARY KEY(id)) AS SELECT * FROM !.!;", [ tested_schema, table_name, tested_schema, tested_table ])
    session.run_sql("ANALYZE TABLE !.!;", [ tested_schema, table_name ])

# prepare the dump
prepare_empty_bucket(OS_BUCKET_NAME, OS_NAMESPACE)
util.dump_schemas([tested_schema], "", {"osBucketName":OS_BUCKET_NAME, "osNamespace": OS_NAMESPACE, "ociConfigFile":oci_config_file})

shell.connect(__sandbox_uri2)

#@<> BUG#33122234 - test
all_read_par=create_par(OS_NAMESPACE, OS_BUCKET_NAME, "AnyObjectRead", "all-read-par", today_plus_days(1, RFC3339), None, "ListObjects")

PREPARE_PAR_IS_SECRET_TEST()
EXPECT_NO_THROWS(lambda: util.load_dump(all_read_par, {"progressFile": ""}), "load_dump() with lots of files")
EXPECT_PAR_IS_SECRET()

EXPECT_STDOUT_CONTAINS(f"{n_tables + 1} tables in 1 schemas were loaded")

#@<> BUG#33122234 - cleanup
session.run_sql("DROP SCHEMA !;", [ tested_schema ])

#@<> Cleanup
testutil.destroy_sandbox(__mysql_sandbox_port1)
testutil.destroy_sandbox(__mysql_sandbox_port2)
