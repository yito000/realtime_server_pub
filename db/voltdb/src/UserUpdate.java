import org.voltdb.*;

public class UserUpdate extends VoltProcedure
{
    private final SQLStmt select_sql = new SQLStmt(
        "select id, uiid from users where id = ? order by id;"
    );
    private final SQLStmt update_sql = new SQLStmt(
        "update users set uiid = ? " + 
        "where id = ?;"
    );
    private final SQLStmt insert_sql = new SQLStmt(
        "insert into users " +
        "(id, uiid) " + 
        "values (?, ?);"
    );

    public VoltTable[] run(int id, String uiid) 
        throws VoltAbortException
    {
        voltQueueSQL(select_sql, id);
        VoltTable[] result = voltExecuteSQL();

        if (result[0].getRowCount() > 0) {
            voltQueueSQL(update_sql, uiid, id);
        } else {
            voltQueueSQL(insert_sql, id, uiid);
        }

        return voltExecuteSQL();
    }
}

