#@ __global__
||

#@<OUT> sqlresult
NAME
      SqlResult - Allows browsing through the result information after
                  performing an operation on the database done through
                  Session.sql

DESCRIPTION
      Allows browsing through the result information after performing an
      operation on the database done through Session.sql

PROPERTIES
      affected_items_count
            Same as get_affected_items_count

      auto_increment_value
            Same as get_auto_increment_value

      column_count
            Same as get_column_count

      column_names
            Same as get_column_names

      columns
            Same as get_columns

      execution_time
            Same as get_execution_time

      warnings
            Same as get_warnings

      warnings_count
            Same as get_warnings_count

FUNCTIONS
      fetch_all()
            Returns a list of DbDoc objects which contains an element for every
            unread document.

      fetch_one()
            Retrieves the next Row on the RowResult.

      fetch_one_object()
            Retrieves the next Row on the result and returns it as an object.

      get_affected_items_count()
            The the number of affected items for the last operation.

      get_auto_increment_value()
            Returns the identifier for the last record inserted.

      get_column_count()
            Retrieves the number of columns on the current result.

      get_column_names()
            Gets the columns on the current result.

      get_columns()
            Gets the column metadata for the columns on the active result.

      get_execution_time()
            Retrieves a string value indicating the execution time of the
            executed operation.

      get_warnings()
            Retrieves the warnings generated by the executed operation.

      get_warnings_count()
            The number of warnings produced by the last statement execution.

      has_data()
            Returns true if the last statement execution has a result set.

      help([member])
            Provides help about this class and it's members

      next_result()
            Prepares the SqlResult to start reading data from the next Result
            (if many results were returned).

#@<OUT> sqlresult.affected_items_count
NAME
      affected_items_count - Same as get_affected_items_count

SYNTAX
      <SqlResult>.affected_items_count

#@<OUT> sqlresult.auto_increment_value
NAME
      auto_increment_value - Same as get_auto_increment_value

SYNTAX
      <SqlResult>.auto_increment_value

#@<OUT> sqlresult.column_count
NAME
      column_count - Same as get_column_count

SYNTAX
      <SqlResult>.column_count

#@<OUT> sqlresult.column_names
NAME
      column_names - Same as get_column_names

SYNTAX
      <SqlResult>.column_names

#@<OUT> sqlresult.columns
NAME
      columns - Same as get_columns

SYNTAX
      <SqlResult>.columns

#@<OUT> sqlresult.execution_time
NAME
      execution_time - Same as get_execution_time

SYNTAX
      <SqlResult>.execution_time

#@<OUT> sqlresult.fetch_all
NAME
      fetch_all - Returns a list of DbDoc objects which contains an element for
                  every unread document.

SYNTAX
      <SqlResult>.fetch_all()

RETURNS
      A List of DbDoc objects.

#@<OUT> sqlresult.fetch_one
NAME
      fetch_one - Retrieves the next Row on the RowResult.

SYNTAX
      <SqlResult>.fetch_one()

RETURNS
      A Row object representing the next record on the result.

#@<OUT> sqlresult.fetch_one_object
NAME
      fetch_one_object - Retrieves the next Row on the result and returns it as
                         an object.

SYNTAX
      <SqlResult>.fetch_one_object()

RETURNS
      A dictionary containing the row information.

DESCRIPTION
      The column names will be used as keys in the returned dictionary and the
      column data will be used as the key values.

      If a column is a valid identifier it will be accessible as an object
      attribute as <dict>.<column>.

      If a column is not a valid identifier, it will be accessible as a
      dictionary key as <dict>[<column>].

#@<OUT> sqlresult.get_affected_items_count
NAME
      get_affected_items_count - The the number of affected items for the last
                                 operation.

SYNTAX
      <SqlResult>.get_affected_items_count()

RETURNS
      the number of affected items.

DESCRIPTION
      Returns the number of records affected by the executed operation.

#@<OUT> sqlresult.get_auto_increment_value
NAME
      get_auto_increment_value - Returns the identifier for the last record
                                 inserted.

SYNTAX
      <SqlResult>.get_auto_increment_value()

DESCRIPTION
      Note that this value will only be set if the executed statement inserted
      a record in the database and an ID was automatically generated.

#@<OUT> sqlresult.get_column_count
NAME
      get_column_count - Retrieves the number of columns on the current result.

SYNTAX
      <SqlResult>.get_column_count()

RETURNS
      the number of columns on the current result.

#@<OUT> sqlresult.get_column_names
NAME
      get_column_names - Gets the columns on the current result.

SYNTAX
      <SqlResult>.get_column_names()

RETURNS
      A list with the names of the columns returned on the active result.

#@<OUT> sqlresult.get_columns
NAME
      get_columns - Gets the column metadata for the columns on the active
                    result.

SYNTAX
      <SqlResult>.get_columns()

RETURNS
      a list of Column objects containing information about the columns
      included on the active result.

#@<OUT> sqlresult.get_execution_time
NAME
      get_execution_time - Retrieves a string value indicating the execution
                           time of the executed operation.

SYNTAX
      <SqlResult>.get_execution_time()

#@<OUT> sqlresult.get_warnings
NAME
      get_warnings - Retrieves the warnings generated by the executed
                     operation.

SYNTAX
      <SqlResult>.get_warnings()

RETURNS
      A list containing a warning object for each generated warning.

DESCRIPTION
      This is the same value than C API mysql_warning_count, see
      https://dev.mysql.com/doc/refman/en/mysql-warning-count.html

      Each warning object contains a key/value pair describing the information
      related to a specific warning.

      This information includes: Level, Code and Message.

#@<OUT> sqlresult.get_warnings_count
NAME
      get_warnings_count - The number of warnings produced by the last
                           statement execution.

SYNTAX
      <SqlResult>.get_warnings_count()

RETURNS
      the number of warnings.

DESCRIPTION
      This is the same value than C API mysql_warning_count, see
      https://dev.mysql.com/doc/refman/en/mysql-warning-count.html

      See get_warnings() for more details.

#@<OUT> sqlresult.has_data
NAME
      has_data - Returns true if the last statement execution has a result set.

SYNTAX
      <SqlResult>.has_data()

#@<OUT> sqlresult.help
NAME
      help - Provides help about this class and it's members

SYNTAX
      <SqlResult>.help([member])

WHERE
      member: If specified, provides detailed information on the given member.

#@<OUT> sqlresult.next_result
NAME
      next_result - Prepares the SqlResult to start reading data from the next
                    Result (if many results were returned).

SYNTAX
      <SqlResult>.next_result()

RETURNS
      A boolean value indicating whether there is another result or not.

#@<OUT> sqlresult.warnings
NAME
      warnings - Same as get_warnings

SYNTAX
      <SqlResult>.warnings

#@<OUT> sqlresult.warnings_count
NAME
      warnings_count - Same as get_warnings_count

SYNTAX
      <SqlResult>.warnings_count

