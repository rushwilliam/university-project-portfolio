package ePortfolio;

/**
 * A mutual fund as an implementation of the investment class
 */
public class MutualFund extends Investment {
    // The cost of selling units
    private static final int COMMISSION = 45;

    /**
     * Create a new mutual fund with the given attributes. The name and symbol must
     * not be blank, price and quantity must be positive
     * 
     * @param symbol   The symbol of the mutual fund
     * @param name     The name of the mutual fund
     * @param quantity The starting number of units owned
     * @param price    The starting price of the mutual fund
     * @throws Exception If one of the given attributes was invalid
     */
    public MutualFund(String symbol, String name, int quantity, float price) throws Exception {
        super(symbol, name, quantity, price);
    }

    /**
     * Create a copy of another mutual fund
     * 
     * @param other The mutual fund to copy
     * @throws Exception If one of the given attributes was invalid
     */
    public MutualFund(MutualFund other) throws Exception {
        super(other.SYMBOL, other.NAME, other.getQuantity(), other.getPrice());
    }

    /**
     * Buy units of this mutual fund
     * 
     * @param quantity The number of units to buy
     * @throws Exception If the number of units to buy is 0 or less
     */
    @Override
    public void buy(int quantity, float price) throws Exception {
        if (quantity <= 0) {
            throw new Exception("Quantity must be positive.");
        }
        // Update the price
        setPrice(price);
        // Add to quantity
        adjustQuantity(quantity);
        // Add to book value
        adjustBookValue(quantity * getPrice());
    }

    /**
     * Sell units of this mutual fund
     * 
     * @param quantity The number of units to sell
     * @return The payment from selling units
     * @throws Exception If the number of units to sell is more than the number of
     *                   currently owned units
     */
    @Override
    public float sell(int quantity, float price) throws Exception {
        if (getQuantity() + quantity < 0) {
            throw new Exception("Cannot sell more shares than owned.");
        }
        // Update the price
        setPrice(price);
        // Adjust book value base on the fraction of units left
        multiplyBookValue((float) (getQuantity() - quantity) / getQuantity());
        // Subtract from quantity
        adjustQuantity(-quantity);
        // Return payment
        return quantity * getPrice() - COMMISSION;
    }

    /**
     * Get the current theoretical gain of this mutual fund if it were to be sold
     * now
     * 
     * @return The current gain of this mutual fund
     */
    @Override
    public float getGain() {
        return getQuantity() * getPrice() - COMMISSION - getBookValue();
    }
}
