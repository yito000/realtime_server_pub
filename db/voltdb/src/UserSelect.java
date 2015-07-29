import org.voltdb.*;

public class UserSelect extends VoltProcedure
{
    private final SQLStmt sql = new SQLStmt(
        "select id, uiid from users where id = ? order by id;"
    );

    public VoltTable[] run(int id) throws VoltAbortException
    {
        voltQueueSQL(sql, id);
        return voltExecuteSQL();
    }
}

