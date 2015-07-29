import org.voltdb.*;

public class UserAdd extends VoltProcedure
{
    private final SQLStmt insert_sql = new SQLStmt(
        "insert into users " +
        "(id, uiid) " +
        "values (?, ?);"
    );

    private final SQLStmt id_select_sql = new SQLStmt(
        "select current_value from identifier where table_name = 'users'"
    );

    private final SQLStmt id_insert_sql = new SQLStmt(
        "insert into identifier (table_name) values (?);"
    );

    private final SQLStmt id_update_sql = new SQLStmt(
        "update identifier set current_value = current_value + 1" +
        "where table_name = ?"
    );

    public VoltTable run(String uiid)
        throws VoltAbortException
    {
        voltQueueSQL(id_select_sql);
        VoltTable[] result = voltExecuteSQL();
        long next_id = 1;

        if (result[0].getRowCount() > 0) {
            VoltTableRow row = result[0].fetchRow(0);
            next_id = row.getLong(0);
        } else {
            voltQueueSQL(id_insert_sql, "users");
        }

        voltQueueSQL(id_update_sql, "users");
        voltQueueSQL(insert_sql, next_id, uiid);
        voltExecuteSQL();

         VoltTable ret = new VoltTable(
            new VoltTable.ColumnInfo("uiid", VoltType.STRING)
        );
        ret.addRow(uiid);

        return ret;
    }
}

