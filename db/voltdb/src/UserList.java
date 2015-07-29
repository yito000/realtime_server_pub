import org.voltdb.*;

public class UserList extends VoltProcedure
{
    private final SQLStmt sql = new SQLStmt(
        "select id, uiid from users order by id;"
    );

    public VoltTable[] run() throws VoltAbortException
    {
        voltQueueSQL(sql);
        return voltExecuteSQL();
    }
}

