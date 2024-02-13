//@ Initialization
||

//@<OUT> RowResult help
NAME
      RowResult - Allows traversing the Row objects returned by a Table.select
                  operation.

DESCRIPTION
      Allows traversing the Row objects returned by a Table.select operation.

PROPERTIES
      affectedItemsCount
            Same as getAffectedItemsCount

      columnCount
            Same as getColumnCount

      columnNames
            Same as getColumnNames

      columns
            Same as getColumns

      executionTime
            Same as getExecutionTime

      warnings
            Same as getWarnings

      warningsCount
            Same as getWarningsCount

FUNCTIONS
      fetchAll()
            Returns a list of DbDoc objects which contains an element for every
            unread document.

      fetchOne()
            Retrieves the next Row on the RowResult.

      fetchOneObject()
            Retrieves the next Row on the result and returns it as an object.

      getAffectedItemsCount()
            The the number of affected items for the last operation.

      getColumnCount()
            Retrieves the number of columns on the current result.

      getColumnNames()
            Gets the columns on the current result.

      getColumns()
            Gets the column metadata for the columns on the active result.

      getExecutionTime()
            Retrieves a string value indicating the execution time of the
            executed operation.

      getWarnings()
            Retrieves the warnings generated by the executed operation.

      getWarningsCount()
            The number of warnings produced by the last statement execution.

      help([member])
            Provides help about this class and it's members

//@<OUT> Help on affectedItemsCount
NAME
      affectedItemsCount - Same as getAffectedItemsCount

SYNTAX
      <RowResult>.affectedItemsCount

//@<OUT> Help on columnCount
NAME
      columnCount - Same as getColumnCount

SYNTAX
      <RowResult>.columnCount

//@<OUT> Help on columnNames
NAME
      columnNames - Same as getColumnNames

SYNTAX
      <RowResult>.columnNames

//@<OUT> Help on columns
NAME
      columns - Same as getColumns

SYNTAX
      <RowResult>.columns

//@<OUT> Help on executionTime
NAME
      executionTime - Same as getExecutionTime

SYNTAX
      <RowResult>.executionTime

//@<OUT> Help on warnings
NAME
      warnings - Same as getWarnings

SYNTAX
      <RowResult>.warnings

//@<OUT> Help on warningsCount
NAME
      warningsCount - Same as getWarningsCount

SYNTAX
      <RowResult>.warningsCount

//@<OUT> Help on fetchAll
NAME
      fetchAll - Returns a list of DbDoc objects which contains an element for
                 every unread document.

SYNTAX
      <RowResult>.fetchAll()

RETURNS
      A List of DbDoc objects.

//@<OUT> Help on fetchOne
NAME
      fetchOne - Retrieves the next Row on the RowResult.

SYNTAX
      <RowResult>.fetchOne()

RETURNS
      A Row object representing the next record on the result.

//@<OUT> Help on fetchOneObject
NAME
      fetchOneObject - Retrieves the next Row on the result and returns it as
                       an object.

SYNTAX
      <RowResult>.fetchOneObject()

RETURNS
      A dictionary containing the row information.

DESCRIPTION
      The column names will be used as keys in the returned dictionary and the
      column data will be used as the key values.

      If a column is a valid identifier it will be accessible as an object
      attribute as <dict>.<column>.

      If a column is not a valid identifier, it will be accessible as a
      dictionary key as <dict>[<column>].

//@<OUT> Help on getAffectedItemsCount
NAME
      getAffectedItemsCount - The the number of affected items for the last
                              operation.

SYNTAX
      <RowResult>.getAffectedItemsCount()

RETURNS
      the number of affected items.

DESCRIPTION
      Returns the number of records affected by the executed operation.

//@<OUT> Help on getColumnCount
NAME
      getColumnCount - Retrieves the number of columns on the current result.

SYNTAX
      <RowResult>.getColumnCount()

RETURNS
      the number of columns on the current result.

//@<OUT> Help on getColumnNames
NAME
      getColumnNames - Gets the columns on the current result.

SYNTAX
      <RowResult>.getColumnNames()

RETURNS
      A list with the names of the columns returned on the active result.

//@<OUT> Help on getColumns
NAME
      getColumns - Gets the column metadata for the columns on the active
                   result.

SYNTAX
      <RowResult>.getColumns()

RETURNS
      a list of Column objects containing information about the columns
      included on the active result.

//@<OUT> Help on getExecutionTime
NAME
      getExecutionTime - Retrieves a string value indicating the execution time
                         of the executed operation.

SYNTAX
      <RowResult>.getExecutionTime()

//@<OUT> Help on getWarnings
NAME
      getWarnings - Retrieves the warnings generated by the executed operation.

SYNTAX
      <RowResult>.getWarnings()

RETURNS
      A list containing a warning object for each generated warning.

DESCRIPTION
      This is the same value than C API mysql_warning_count, see
      https://dev.mysql.com/doc/refman/en/mysql-warning-count.html

      Each warning object contains a key/value pair describing the information
      related to a specific warning.

      This information includes: Level, Code and Message.

//@<OUT> Help on getWarningsCount
NAME
      getWarningsCount - The number of warnings produced by the last statement
                         execution.

SYNTAX
      <RowResult>.getWarningsCount()

RETURNS
      the number of warnings.

DESCRIPTION
      This is the same value than C API mysql_warning_count, see
      https://dev.mysql.com/doc/refman/en/mysql-warning-count.html

      See getWarnings() for more details.

//@<OUT> Help on help
NAME
      help - Provides help about this class and it's members

SYNTAX
      <RowResult>.help([member])

WHERE
      member: If specified, provides detailed information on the given member.

//@ Finalization
||
