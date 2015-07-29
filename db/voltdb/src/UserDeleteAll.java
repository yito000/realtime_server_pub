import org.voltdb.*;

public class UserDeleteAll extends VoltProcedure
{
    private final SQLStmt sql = new SQLStmt(
        "delete from users;"
    );

    public VoltTable[] run() throws VoltAbortException
    {
        voltQueueSQL(sql);
        return voltExecuteSQL();
    }
}

