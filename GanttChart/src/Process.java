public class Process {
    private final String pid;
    private final int start;
    private final int end;


    public Process(String pid, int start, int end) {
        this.pid = pid;
        this.start = start;
        this.end = end;
    }

    public String getPid() {
        return pid;
    }

    public int getStart() {
        return start;
    }

    public int getEnd() {
        return end;
    }
}
