package ePortfolio;

import java.util.ArrayList;
import java.util.HashMap;

/**
 * Methods for managing the investment and keyword data
 */
public class DataManager {
    /**
     * All investments with at least one unit owned
     */
    private ArrayList<Investment> investments;
    /**
     * Hashmap of all keywords in the names of investments
     */
    private HashMap<String, ArrayList<Integer>> keywords;

    /**
     * Set up data structures
     */
    public DataManager() {
        investments = new ArrayList<>();
        keywords = new HashMap<>();
    }

    /**
     * Create a new investment or buy more of an investment if one with the given
     * symbol already exists
     * 
     * @param type     The type of the investment
     * @param symbol   The symbol of the investment
     * @param name     The name of the investment
     * @param quantity The quantity of the investment to buy
     * @param price    The current price of the investment
     * @return Feedback about the action performed
     * @throws Exception If there was an error with some part of the transaction
     */
    public String buyInvestment(String type, String symbol, String name, String quantity, String price)
            throws Exception {
        int numQuantity;
        float numPrice;
        // Try converting quantity to an integer
        try {
            numQuantity = Integer.parseInt(quantity);
        } catch (Exception e) {
            throw new Exception("Quantity must be an integer.");
        }
        // Try converting price to a float
        try {
            numPrice = Float.parseFloat(price);
        } catch (Exception e) {
            throw new Exception("Price must be a number.");
        }
        // Try getting the investment by symbol
        Investment investment = getInvestment(symbol);
        if (investment == null) {
            // Create a new investment and add the symbol
            addKeywords(investments.size(), name);
            if (type.equals("Stock")) {
                investments.add(new Stock(symbol, name, numQuantity, numPrice));
            } else if (type.equals("Mutual Fund")) {
                investments.add(new MutualFund(symbol, name, numQuantity, numPrice));
            }
            return "Created new " + type + ": " + name + " (" + symbol + ").";
        } else {
            // Buy more of an existing investment
            investment.buy(numQuantity, numPrice);
            return "Purchased " + numQuantity + " units of " + investment.NAME + " (" + investment.SYMBOL + ").";
        }
    }

    /**
     * Sell more of an existing investment
     * 
     * @param symbol   The symbol of the investment
     * @param quantity The quantity of the investment to sell
     * @param price    The current price of the investment
     * @return Feedback about the action performed
     * @throws Exception If there was an error with some part of the transaction
     */
    public String sellInvestment(String symbol, String quantity, String price) throws Exception {
        int numQuantity;
        float numPrice;
        // Try converting quantity to an integer
        try {
            numQuantity = Integer.parseInt(quantity);
        } catch (Exception e) {
            throw new Exception("Quantity must be an integer.");
        }
        // Try converting price to a float
        try {
            numPrice = Float.parseFloat(price);
        } catch (Exception e) {
            throw new Exception("Price must be a number.");
        }
        // Try getting the investment by symbol
        Investment investment = getInvestment(symbol);
        if (investment == null) {
            throw new Exception("An investment with that symbol does not exist");
        }
        // Sell units of the investment
        investment.sell(numQuantity, numPrice);
        // If there are no units left of the investment, remove it
        if (investment.getQuantity() == 0) {
            // Remove the investment and its keywords
            removeKeywords(investments.indexOf(investment), investment.NAME);
            investments.remove(investment);
            return "Sold " + numQuantity + " units of " + investment.NAME + " (" + investment.SYMBOL
                    + ").\nAll units have been sold. Removing investment " + investment.NAME + " (" + investment.SYMBOL
                    + ").";
        }
        return "Sold " + numQuantity + " units of " + investment.NAME + " (" + investment.SYMBOL + ").";
    }

    /**
     * Get the arraylist containing the investments
     * 
     * @return All of the investments in the system
     */
    public ArrayList<Investment> getAllInvestments() {
        return investments;
    }

    /**
     * Calculate and return the total gain of all investments
     * 
     * @return The total gain of all investments
     */
    public String getTotalGain() {
        // The total gain
        float totalGain = 0;
        for (Investment investment : investments) {
            totalGain += investment.getGain();
        }
        // Return the total gain rounded to two decimal places
        return String.format("%.2f", totalGain);
    }

    /**
     * Get all of the current gains of the investments
     * 
     * @return A formatted string containing all of the individual gains of the
     *         investments
     */
    public String getIndividualGains() {
        // The output string
        String output = "";
        // Add information from all investments to the formatted output
        for (Investment investment : investments) {
            output += "Gain for " + investment.NAME + " (" + investment.SYMBOL + "): "
                    + String.format("%.2f", investment.getGain()) + "\n";
        }
        return output;
    }

    /**
     * Attempt to get an investment by its symbol
     * 
     * @param symbol The symbol to search for
     * @return The investment with that symbol, or null if one cannot be found
     */
    private Investment getInvestment(String symbol) {
        // Search through all investments
        for (Investment investment : investments) {
            if (symbol.equals(investment.SYMBOL)) {
                return investment;
            }
        }
        // If that investment could not be found
        return null;
    }

    /**
     * Add the keywords in a string to the database of keywords. Keywords are stored
     * as a key value pair. The key is a string representing the word, and the value
     * is an arraylist of all of the investment indexes in which that word appears
     * 
     * @param index The index of the investment
     * @param name  The keywords to store
     */
    private void addKeywords(int index, String name) {
        // For all words in name
        for (String word : name.toLowerCase().split("[ ]+")) {
            // If the word does not already exist, create it
            if (!keywords.containsKey(word)) {
                keywords.put(word, new ArrayList<Integer>());
            }
            // Add the index to all indexes
            keywords.get(word).add(index);

        }
    }

    /**
     * Remove the keywords of a string from the map
     * 
     * @param index The index of the string
     * @param name  The string of keywords to remove
     */
    private void removeKeywords(int index, String name) {
        // For all words in name
        for (String word : name.toLowerCase().split("[ ]+")) {
            // Get the indexes and remove the index of the investment being removed
            ArrayList<Integer> indexes = keywords.get(word);
            indexes.remove(indexes.indexOf(index));
            // Remove the word if there are no indexes remaining
            if (indexes.size() == 0) {
                keywords.remove(word);
            }
        }
        // Iterate through all indexes
        for (ArrayList<Integer> indexes : keywords.values()) {
            for (int i : indexes)
                // Decrement the index if it is greater than the index being removeds
                if (index < i) {
                    indexes.set(indexes.indexOf(i), i - 1);
                }
        }
    }

    /**
     * Search through all investments and get a formatting string containing
     * information about all investments that match the search criteria. All search
     * criteria are optional.
     * 
     * @param symbol    The symbol of the investment
     * @param name      The name of the investment
     * @param lowPrice  The lower bound of the investment
     * @param highPrice The upper bound of the investment
     * @return A formatted string containing all investments that match the search
     *         criteria, or a message that no investments could be found
     * @throws Exception If there were any errors with the information given
     */
    public String search(String symbol, String name, String lowPrice, String highPrice) throws Exception {
        float numLowPrice = 0;
        // Try converting the lower bound to a float
        if (!lowPrice.isBlank()) {
            try {
                numLowPrice = Float.parseFloat(lowPrice);
            } catch (Exception e) {
                throw new Exception("Low price must be a number.");
            }
        }
        // A default value of -1 signifies infinity
        float numHighPrice = -1;
        // Try converting the upper bound to a float
        if (!highPrice.isBlank()) {
            try {
                numHighPrice = Float.parseFloat(highPrice);
            } catch (Exception e) {
                throw new Exception("High price must be a number.");
            }
            // Since this specific error wont be caught by the check later
            if (numHighPrice == -1) {
                throw new Exception("Price range cannot be less than 0");
            }
        }
        // If either price options is negative
        if (numLowPrice < 0 || (numHighPrice < 0 && numHighPrice != -1)) {
            throw new Exception("Price range cannot be less than 0");
        }
        // If the lower price is higher than the higher price
        if (numLowPrice > numHighPrice && numHighPrice != -1) {
            throw new Exception("High price cannot be less than low price");
        }
        String output = "";
        // Use a blank array by default
        String[] nameKeywords = {};
        if (!name.isBlank()) {
            nameKeywords = name.toLowerCase().split("[ ]+");
        }
        // Search through investments
        for (Investment investment : investments) {
            if (matchInvestment(investment, symbol, nameKeywords, numLowPrice, numHighPrice)) {
                output += investment;
            }
        }
        // Return the formatted output or a message that no investments could be found
        return output.isBlank() ? "No investments matched that criteria" : output;
    }

    /**
     * Compares the given investment to a set of information, and return if that
     * investment matches the parameters
     * 
     * @param investment The investment object to check
     * @param symbol     Optional symbol to match exactly
     * @param keywords   Set of keywords to find in the investment name, any order,
     *                   not case sensitive
     * @param minimum    Minimum value of price
     * @param maximum    Maximum value of price (-1 for no maximum value)
     * @return If the investment matches the parameters
     */
    private boolean matchInvestment(Investment investment, String symbol, String[] words, float minimum,
            float maximum) {
        // If a symbol was given that does not match the investment symbol, return false
        if (!symbol.isBlank() && !investment.SYMBOL.equals(symbol)) {
            return false;
        }
        // Match all words given
        for (String word : words) {
            // Indexes if the word
            ArrayList<Integer> indexes;
            // If the word does not exist in the database
            if ((indexes = keywords.get(word)) == null) {
                return false;
            }
            // If the index of the investment is not in the indexes in the database
            if (indexes.indexOf(investments.indexOf(investment)) == -1) {
                return false;
            }
        }
        // If the investment price is not within the range, return false. If maximum is
        // -1, ignore maximum
        if ((maximum != -1 && investment.getPrice() > maximum) || investment.getPrice() < minimum) {
            return false;
        }
        // All checks were passed, investment matches
        return true;
    }
}
