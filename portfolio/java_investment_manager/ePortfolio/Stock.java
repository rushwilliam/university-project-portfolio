package ePortfolio;

/**
 * A stock as an implementation of the investment class shares
 */
public class Stock extends Investment {
    // The cost of making a transaction
    private static final float COMMISSION = 9.99f;

    /**
     * Create a new stock with the given attributes. The name and symbol must
     * not be blank, price and quantity must be positive
     * 
     * @param symbol   The symbol of the stock
     * @param name     The name of the stock
     * @param quantity The starting number of shares owned
     * @param price    The starting price of the stock
     * @throws Exception If one of the given attributes was invalid
     */
    public Stock(String symbol, String name, int quantity, float price) throws Exception {
        super(symbol, name, quantity, price);
    }

    /**
     * Create a copy of another stock
     * 
     * @param other The stock to copy
     * @throws Exception If one of the given attributes was invalid
     */
    public Stock(Stock other) throws Exception {
        super(other.SYMBOL, other.NAME, other.getQuantity(), other.getPrice());
    }

    /**
     * Buy shares of this stock
     * 
     * @param quantity The number of shares to buy
     * @throws Exception If the number of shares to buy is 0 or less
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
        adjustBookValue(quantity * getPrice() + COMMISSION);
    }

    /**
     * Sell shares of this stock
     * 
     * @param quantity The number of shares to sell
     * @return The payment from selling shares
     * @throws Exception If the number of shares to sell is more than the number of
     *                   currently owned shares
     */
    @Override
    public float sell(int quantity, float price) throws Exception {
        // This exception is redundant, as the same value is checked in the
        // adjustQuantity function. This ensures that the setPrice function is not
        // called if the quantity value is invalid, so that the current price is not
        // changed. This does not have to be done for the price, as it is the first
        // value to be changed, and if it is invalid then the other functions will not
        // be called (At least I think, I'm tired)
        if (getQuantity() - quantity < 0) {
            throw new Exception("Cannot sell more shares than owned.");
        }
        // Update the price
        setPrice(price);
        // Adjust book value base on the fraction of shares left
        multiplyBookValue((float) (getQuantity() - quantity) / getQuantity());
        // Subtract from quantity
        adjustQuantity(-quantity);
        // Return payment
        return quantity * getPrice() - COMMISSION;
    }

    /**
     * Get the current theoretical gain of this stock if it were to be sold now
     * 
     * @return The current gain of this stock
     */
    @Override
    public float getGain() {
        return getQuantity() * getPrice() - COMMISSION - getBookValue();
    }
}
