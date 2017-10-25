import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.util.Arrays;
import java.util.Comparator;
import java.util.HashMap;
import java.util.HashSet;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;
import java.util.Stack;
import java.util.StringTokenizer;
import java.util.function.ToDoubleFunction;

// You should call this code as follows:
//
//   java WebSearch directoryName searchStrategyName
//   (or jview, in J++)
//
//   where <directoryName> is the name of corresponding intranet
//   and <searchStrategyName> is one of {breadth, depth, best, beam}.

// The PARTIAL code below contains code for fetching and parsing
// the simple web pages we're using, as well as the fragments of
// a solution.  BE SURE TO READ ALL THE COMMENTS.

// Feel free to alter or discard whatever code you wish;
// the only requirement is that your main class be called WebSearch
// and that it accept the two arguments described above
// (if you wish you can add additional OPTIONAL arguments, but they
// should default to the values "hardwired" in below).

public class WebSearch {
	private static LinkedList<SearchNode> open;
	private static HashSet<String> closed;
	private static Map<String, Double> pageDist;

	// When set, report what's happening.
  // WARNING: lots of info is printed.
	private static final boolean DEBUGGING = false;

	private static final int BEAM_WIDTH = 10; // If searchStrategy = "beam",
	// limit the size of open to this value.
	// The setSize() method in the Vector
	// class can be used to accomplish this.

	private static final String START_NODE = "page1.html";

	// A web page is a goal node if it includes
	// the following string.
	private static final String GOAL_PATTERN = "QUERY1 QUERY2 QUERY3 QUERY4";
	private static final List<String> QUERY_WORDS = Arrays.asList(GOAL_PATTERN.split(" "));

	public static void main(String args[]) {
		if (args.length != 2) {
			System.out.println("You must provide the directoryName and searchStrategyName. Please try again.");
		} else {
			String directoryName = args[0]; // Read the search strategy to use.
			String searchStrategyName = args[1]; // Read the search strategy to use.

			if (
			  searchStrategyName.equalsIgnoreCase("breadth") ||
				searchStrategyName.equalsIgnoreCase("depth")   ||
				searchStrategyName.equalsIgnoreCase("best")    ||
				searchStrategyName.equalsIgnoreCase("beam")
			) {
				performSearch(START_NODE, directoryName, searchStrategyName);
			} else {
				System.out.println("The valid search strategies are:");
				System.out.println("  breadth, depth, best, beam");
			}
		}

		Utilities.waitHere("Press ENTER to exit.");
	}

	static void performSearch(
	  String startNode,
		String directoryName,
		String searchStrategy
	) {
		int nodesVisited = 0;

		open   = new LinkedList<SearchNode>();
		closed = new HashSet<String>();
		pageDist = new HashMap<>();

		open.add(new SearchNode(startNode));

		while (!open.isEmpty()) {
			if (DEBUGGING) System.out.println("Open is: " + open);

			SearchNode currentNode = pop(open);
			String currentURL = currentNode.getNodeName();

			nodesVisited++;

			// Go and fetch the contents of this file.
			if (DEBUGGING) System.out.printf("Visiting %s\n", currentURL);
			String contents = Utilities.getFileContents(
			  directoryName + File.separator + currentURL
			);

			if (isaGoalNode(contents)) {
				// Report the solution path found
				// (You might also wish to write a method that
				// counts the solution-path's length, and then print that
				// number here.)
				currentNode.reportPathToGoal();
				break;
			}

			// Remember this node was visited.
			closed.add(currentURL);
			pageDist.put(currentURL, pageH(contents));
			// System.out.printf("Page: %s\t Score: %f\n", currentURL, pageDist.get(currentURL));
			addNewChildrenToOpen(currentNode, contents, searchStrategy);

			// Provide a status report.
			if (DEBUGGING) System.out.println("Nodes visited = " + nodesVisited
					+ " |open| = " + open.size());
		}

		System.out.printf(
		  "Visited %d nodes, starting @%s, using strategy '%s' \n",
			nodesVisited,
			directoryName + File.separator + startNode,
			searchStrategy
		);
	}

	// This method reads the page's contents and
	// collects the 'children' nodes (ie, the hyperlinks on this page).
	// The parent node is also passed in so that 'backpointers' can be
	// created (in order to later extract solution paths).
	static void addNewChildrenToOpen(
	  SearchNode parent,
		String contents,
		String searchStrategy
	) {
		// StringTokenizer's are a nice class built into Java.
		// Be sure to read about them in some Java documentation.
		// They are useful when one wants to break up a string into words (tokens).
		StringTokenizer st = new StringTokenizer(contents);
		int numberOfNewNodes = 0;

		while (st.hasMoreTokens()) {
			String token = st.nextToken();

			// Look for the hyperlinks on the current page.
			// (Lots a print statments and error checks are in here,
			// both as a form of documentation and as a debugging tool should you
			// create your own intranets.)

			// At the start of some hypertext?  Otherwise, ignore this token.
			if (token.equalsIgnoreCase("<A")) {
				String hyperlink; // The name of the child node.

				if (DEBUGGING) System.out.println("Encountered a HYPERLINK");

				// Read: HREF = page#.html >
				token = st.nextToken();
				if (!token.equalsIgnoreCase("HREF")) {
					System.out.println("Expecting 'HREF' and got: " + token);
				}

				token = st.nextToken();
				if (!token.equalsIgnoreCase("=")) {
					System.out.println("Expecting '=' and got: " + token);
				}

				// Now we should be at the name of file being linked to.
				hyperlink = st.nextToken();
				if (!hyperlink.startsWith("page")) {
					System.out.println("Expecting 'page#.html' and got: " + hyperlink);
				}

				token = st.nextToken();
				if (!token.equalsIgnoreCase(">")) {
					System.out.println("Expecting '>' and got: " + token);
				}

				if (DEBUGGING) System.out.println(" - found a link to " + hyperlink);

				//////////////////////////////////////////////////////////////////////
				// Have collected a child node; now have to decide what to do with it.
				//////////////////////////////////////////////////////////////////////

				// If already in open, we'll ignore this hyperlink
				// (Be sure to read the "Technical Note" below.)
				if (alreadyInOpen(hyperlink)) {
					if (DEBUGGING) System.out.println(" - this node is in the open list.");
				} else if (closed.contains(hyperlink)) {
					// If already in closed, we'll also ignore this hyperlink.
					if (DEBUGGING) System.out.println(" - this node is in the closed list.");
				} else {
					// Collect the hypertext if this is a previously unvisited node.
					// (This is only needed for HEURISTIC SEARCH, but collect in
					// all cases for simplicity.)
					String hypertext = ""; // The text associated with this hyperlink.

					do {
						token = st.nextToken();
						if (!token.equalsIgnoreCase("</A>")) hypertext += " " + token;
					} while (!token.equalsIgnoreCase("</A>"));

					if (DEBUGGING) System.out.println("   with hypertext: " + hypertext);

					//////////////////////////////////////////////////////////////////////
					// At this point, you have a new child (hyperlink) and you have to
					// insert it into open according to the search strategy being used.
					// Your heuristic function for best-first search should accept as
					// arguments both "hypertext" (ie, the text associated with this
					// hyperlink) and "contents" (ie, the full text of the current page).
					//////////////////////////////////////////////////////////////////////

					// Create the search node.
					SearchNode newNode = new SearchNode(hyperlink, hypertext, parent);

					if (searchStrategy.equals("breadth")) {
						open.add(newNode);
					} else if(searchStrategy.equals("depth")) {
						open.add(0, newNode);
					} else if (
					  searchStrategy.equals("best") ||
						searchStrategy.equals("beam")
					) {
						// System.out.printf("New node h-scores:\t%f\t%f\t%f\n", h1(newNode), h2(newNode), h3(newNode));
						open.add(newNode);
						open.sort(new HeuristicComparator(WebSearch::h3));

						if (searchStrategy.equals("beam") && open.size() > BEAM_WIDTH) {
							// Keep only the first 'BEAM_WIDTH' elements.
							open.subList(BEAM_WIDTH, open.size()).clear();
						}
					} else {
						System.err.printf(
						  "Search strategy '%s' not yet implemented.\n",
							searchStrategy
						);
					}

					// Technical note: in best-first search,
					// if a page contains TWO (or more) links to the SAME page,
					// it is acceptable if only the FIRST one is inserted into open,
					// rather than the better-scoring one.  For simplicity, once a node
					// has been placed in open or closed, we won't worry about the
					// possibility of later finding of higher score for it.
					// Since we are scoring the hypertext POINTING to a page,
					// rather than the web page itself, we are likely to get
					// different scores for given web page.  Ideally, we'd
					// take this into account when sorting open, but you are
					// NOT required to do so (though you certainly are welcome
					// to handle this issue).

					// HINT: read about the insertElementAt() and addElement()
					// methods in the Vector class.
				}
			}
		}
	}

	public static double pageH(String contents) {
		StringTokenizer st = new StringTokenizer(contents);
		int totalWords = 0;
		int queryWords = 0;

		while (st.hasMoreTokens()) {
			String token = st.nextToken();

			// At the start of some hypertext?  Otherwise, ignore this token.
			if (token.equalsIgnoreCase("<A")) {
				String hyperlink; // The name of the child node.

				if (DEBUGGING) System.out.println("Encountered a HYPERLINK");

				// Read: HREF = page#.html >
				token = st.nextToken();
				if (!token.equalsIgnoreCase("HREF")) {
					System.out.println("Expecting 'HREF' and got: " + token);
				}

				token = st.nextToken();
				if (!token.equalsIgnoreCase("=")) {
					System.out.println("Expecting '=' and got: " + token);
				}

				// Now we should be at the name of file being linked to.
				hyperlink = st.nextToken();
				if (!hyperlink.startsWith("page")) {
					System.out.println("Expecting 'page#.html' and got: " + hyperlink);
				}

				token = st.nextToken();
				if (!token.equalsIgnoreCase(">")) {
					System.out.println("Expecting '>' and got: " + token);
				}
			} else if (!token.equals("</A>")) {
				if (QUERY_WORDS.contains(token)) {
					queryWords++;
				}
				totalWords++;
			}
		}

		return 1 - (double)queryWords / totalWords;
	}

	// Scores a SearchNode using the following heuristic:
	//   h(n) in the range [0,1] is the proportion of
	//   words in the hypertext of n that are NOT query words
	public static double h1(SearchNode n) {
		List<String> nodeWords = Arrays.asList(n.getHyperText().split(" "));
		double misses = 0;

		for (String word : nodeWords) {
			if (!QUERY_WORDS.contains(word)) {
				misses++;
			}
		}

		return misses / nodeWords.size();
	}

	private static final double CONSECUTIVE_PENALTY = .4;
	private static final double ORDER_PENALTY = .4;
	public static double h2(SearchNode n) {
		List<String> nodeWords = Arrays.asList(n.getHyperText().split(" "));
    int lastQueryWord = -1;
		boolean consecutive = true;
		int size = nodeWords.size();
		double dist = 0;

		for (int i = 0, end = nodeWords.size(); i < end; i++) {
			int index = QUERY_WORDS.indexOf(nodeWords.get(i));

			if (index == -1) {
				// Not a query word.
				// Update the distance score.
				dist += 1;

				// Update bookkeeping variables.
				lastQueryWord = -1;
				consecutive = false;
			} else {
				// Query word.
				// Update the distance score.
				dist +=
					// Is this the 'next' query word?
					(index == lastQueryWord + 1 ? 0 : ORDER_PENALTY) +
					// Was the previous word a query word?
					(consecutive ? 0 : CONSECUTIVE_PENALTY);

				// Update bookkeeping variables.
				lastQueryWord = index;
				consecutive = true;
			}
		}

		return dist / size;
	}

	private static final double PAGE_PROPORTION = 0.5;
	public static double h3(SearchNode n) {
		double linkScore = h2(n);
		double pageScore = pageDist.get(n.getParent().getNodeName());

		return linkScore * (1 - PAGE_PROPORTION) + pageScore * PAGE_PROPORTION;
	}

	// A GOAL is a page that contains the goalPattern set above.
	static boolean isaGoalNode(String contents) {
		return (contents != null && contents.indexOf(GOAL_PATTERN) >= 0);
	}

	// Is this hyperlink already in the open list?
	// This isn't a very efficient way to do a lookup,
	// but its fast enough for this homework.
	// Also, this for-loop structure can be
	// be adapted for use when inserting nodes into open
	// according to their heuristic score.
	static boolean alreadyInOpen(String hyperlink) {
		int length = open.size();

		for(int i = 0; i < length; i++){
			SearchNode node = open.get(i);
			String oldHyperlink = node.getNodeName();

			if (hyperlink.equalsIgnoreCase(oldHyperlink)) return true;  // Found it.
		}

		return false;  // Not in open.
	}

	// You can use this to remove the first element from open.
	static SearchNode pop(LinkedList<SearchNode> list) {
		return list.removeFirst();
	}
}

class HeuristicComparator implements Comparator<SearchNode> {
	private ToDoubleFunction<SearchNode> heuristic;

	public HeuristicComparator(ToDoubleFunction<SearchNode> h) {
		heuristic = h;
	}

	public int compare(SearchNode a, SearchNode b) {
		return Double.compare(
		  heuristic.applyAsDouble(a),
			heuristic.applyAsDouble(b)
		);
	}
}

/////////////////////////////////////////////////////////////////////////////////

// You'll need to design a Search node data structure.

// Note that the above code assumes there is a method called getHvalue()
// that returns (as a double) the heuristic value associated with a search node,
// a method called getNodeName() that returns (as a String)
// the name of the file (eg, "page7.html") associated with this node, and
// a (void) method called reportPathToGoal() that prints the path
// from the start node to the current node represented by the SearchNode instance.
class SearchNode {
	private final SearchNode parent;
	private final String nodeName;
	private final String hyperText;

	public SearchNode(String name) {
		this(name, null, null);
	}

	public SearchNode(String name, SearchNode parent) {
		this(name, null, parent);
	}

	public SearchNode(String name, String hyperText, SearchNode parent) {
		nodeName = name;
		this.hyperText = hyperText;
		this.parent = parent;
	}

	public boolean isSamePage(SearchNode s) {
		return s.nodeName.equals(nodeName);
	}

	public String getHyperText() {
		return hyperText;
	}

	public SearchNode getParent() {
		return parent;
	}

	public void reportPathToGoal() {
		Stack<SearchNode> path = new Stack<>();
		SearchNode current = this;
		int length = -1;

		while(current != null) {
			length++;
			path.push(current);
			current = current.parent;
		}

		System.out.printf("Path from start to goal (length %d):\n", length);
		boolean first = true;
		while(!path.empty()) {
			current = path.pop();
			System.out.printf(
			  "  %s %s\n",
				current.nodeName,
				first ? "(start)" : path.empty() ? "(goal)" : ""
			);
			first = false;
		}
	}

	public String getNodeName() {
		return nodeName;
	}
}

/////////////////////////////////////////////////////////////////////////////////

// Some 'helper' functions follow.  You needn't understand their internal details.
// Feel free to move this to a separate Java file if you wish.
class Utilities {
	// In J++, the console window can close up before you read it,
	// so this method can be used to wait until you're ready to proceed.
	public static void waitHere(String msg) {
		System.out.println("");
		System.out.println(msg);
		try { System.in.read(); } catch(Exception e) {} // Ignore any errors while reading.
	}

	// This method will read the contents of a file, returning it
	// as a string.  (Don't worry if you don't understand how it works.)
	public static synchronized String getFileContents(String fileName) {
		File file = new File(fileName);
		String results = null;

		try {
			int length = (int)file.length(), bytesRead;
			byte byteArray[] = new byte[length];

			ByteArrayOutputStream bytesBuffer = new ByteArrayOutputStream(length);
			FileInputStream       inputStream = new FileInputStream(file);
			bytesRead = inputStream.read(byteArray);
			bytesBuffer.write(byteArray, 0, bytesRead);
			inputStream.close();

			results = bytesBuffer.toString();
		} catch(IOException e) {
			System.out.println("Exception in getFileContents(" + fileName + "), msg=" + e);
		}

		return results;
	}
}
