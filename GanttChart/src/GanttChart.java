import org.jfree.chart.ChartFactory;
import org.jfree.chart.ChartPanel;
import org.jfree.chart.JFreeChart;
import org.jfree.chart.axis.DateAxis;
import org.jfree.chart.plot.CategoryPlot;
import org.jfree.data.category.IntervalCategoryDataset;
import org.jfree.data.gantt.Task;
import org.jfree.data.gantt.TaskSeries;
import org.jfree.data.gantt.TaskSeriesCollection;
import org.jfree.data.time.SimpleTimePeriod;

import javax.swing.*;
import java.awt.*;
import java.io.File;
import java.io.FileNotFoundException;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashMap;
import java.util.List;
import java.util.Scanner;

public class GanttChart extends JFrame {
    public static void main(String[] args) throws FileNotFoundException {
        ArrayList<Process> process_list = new ArrayList<>();

        File file = new File("src/gantt.txt");
        Scanner scanner = new Scanner(file);
        int clock = 0;

        while (scanner.hasNextLine()) {
            String line = scanner.nextLine();
            List<String> list = Arrays.asList(line.split(","));
            process_list.add(new Process(list.get(0), Integer.parseInt(list.get(1)), Integer.parseInt(list.get(2))));

            if(!scanner.hasNextLine()) {
                clock = Integer.parseInt(list.get(2));
//                clock = Integer.parseInt(line);
            }
        }

//        System.out.println(clock);

        HashMap<String, ArrayList<Process>> process_map = parseProcessListToMap(process_list);

        GanttChart chart = new GanttChart("Scheduler Gantt Chart", "MFQS", process_map, clock);
        chart.pack();
        chart.setVisible(true);
    }

    private static HashMap<String, ArrayList<Process>> parseProcessListToMap(ArrayList<Process> process_list) {
        HashMap<String, ArrayList<Process>> process_map = new HashMap<>();
        for (Process process : process_list) {

            ArrayList<Process> map_list = process_map.get(process.getPid());

            if (map_list == null) {
                map_list = new ArrayList<>();
            }
            map_list.add(process);
            process_map.put(process.getPid(), map_list);
        }

//        for (ArrayList<Process> list : process_map.values()) {
//            System.out.print(list.get(0).getPid() + ": [");
//            for (Process process : list) {
//                System.out.print("{" + process.getPid() + "; " + process.getStart() + "; " + process.getEnd() + "}, ");
//            }
//            System.out.println("]");
//        }

        return process_map;
    }

    public GanttChart(String applicationTitle, String chartTitle, HashMap<String, ArrayList<Process>> processes, int clock) {
        super(applicationTitle);

        JFreeChart chart = ChartFactory.createGanttChart(chartTitle, "PID", "Clock Ticks", createDataSet(processes, clock), false, false, false);
        CategoryPlot plot = chart.getCategoryPlot();

        DateAxis axis = (DateAxis) plot.getRangeAxis();
        axis.setDateFormatOverride(new SimpleDateFormat("SS"));
        ChartPanel chartPanel = new ChartPanel(chart);

        chartPanel.setPreferredSize(new Dimension(1000, 600));

        setContentPane(chartPanel);
    }

    private IntervalCategoryDataset createDataSet(HashMap<String, ArrayList<Process>> process_map, int clock) {
        TaskSeriesCollection dataset = new TaskSeriesCollection();
        TaskSeries series = new TaskSeries("Processes");

        for (ArrayList<Process> processes : process_map.values()) {
            if (processes.size() > 1) {
                Task task = new Task(processes.get(0).getPid(), new SimpleTimePeriod(0, clock));
                for (int i = 0; i < processes.size(); i++) {
                    Process p = processes.get(i);
                    task.addSubtask(new Task(p.getPid() + "-" + i, new SimpleTimePeriod(p.getStart(), p.getEnd())));
                }
                series.add(task);
            } else {
                Process p = processes.get(0);
                Task task = new Task(p.getPid(), new SimpleTimePeriod(p.getStart(), p.getEnd()));
                series.add(task);
            }
        }

        dataset.add(series);
        return dataset;
    }
}
