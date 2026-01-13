package ePortfolio;

/**
 * Abstract class representing an investment. Provides basic methods for
 * adjusting values, specific methods for actions performed by the user must be
 * implemented
 */
public abstract class Investment {
    /**
     * The symbol of the investment
     */
    public final String SYMBOL;
    /**
     * The name of the investment
     */
    public final String NAME;
    /**
     * The current number of units owned
     */
    private int quantity;
    /**
     * The current price of the investment, given by the user
     */
    private float price;
    /**
     * The current book value of the investment
     */
    private float bookValue;

    /**
     * Create a new investment with the given attributes. The name and symbol must
     * not be blank, price and quantity must be positive
     * 
     * @param symbol   The symbol of the investment
     * @param name     The name of the investment
     * @param quantity The starting number of units owned
     * @param price    The starting price of the investment
     * @throws Exception If one of the given attributes was invalid
     */
    public Investment(String symbol, String name, int quantity, float price) throws Exception {
        // If no symbol was given
        if (symbol.isBlank()) {
            throw new Exception("Symbol cannot be blank.");
        }
        // If symbol is multiple tokens
        if (symbol.split("[ ]+").length != 1) {
            throw new Exception("Symbol must be one token");
        }
        // If no name was given
        if (name.isBlank()) {
            throw new Exception("Name cannot be blank.");
        }
        // If the starting quantity was 0 or less
        if (quantity <= 0) {
            throw new Exception("Starting quantity must be greater than 0.");
        }
        // Set attributes
        this.SYMBOL = symbol;
        this.NAME = name;
        buy(quantity, price);
    }

    /**
     * Create a copy of another investment
     * 
     * @param other THe investment to copy
     */
    public Investment(Investment other) {
        // Copy attributes
        SYMBOL = other.SYMBOL;
        NAME = other.NAME;
        quantity = other.quantity;
        price = other.price;
        bookValue = other.bookValue;
    }

    /**
     * Adjust the quantity by an amount
     * 
     * @param quantityChange How much to change quantity by
     * @throws Exception If the change in quantity would result in a negative value
     */
    public void adjustQuantity(int quantityChange) throws Exception {
        if (this.quantity + quantityChange < 0) {
            throw new Exception("Cannot sell more shares than owned.");
        }
        this.quantity += quantityChange;
    }

    /**
     * Get the current number of units owned
     * 
     * @return The current number of units owned
     */
    public int getQuantity() {
        return quantity;
    }

    /**
     * Set the current price of the investment
     * 
     * @param price The current price of the investment
     * @throws Exception If the new price is less than 0
     */
    public void setPrice(float price) throws Exception {
        if (price < 0) {
            throw new Exception("Price cannot be less than 0.");
        }
        this.price = price;
    }

    /**
     * Get the current price of the investment
     * 
     * @return The current price of the investment
     */
    public float getPrice() {
        return price;
    }

    /**
     * Adjust book value by an amount
     * 
     * @param bookValueChange The amount to change book value by
     */
    public void adjustBookValue(float bookValueChange) {
        bookValue += bookValueChange;
    }

    /**
     * Multiply book value by an amount
     * 
     * @param bookValueRatio The ratio to multiply book value by
     */
    public void multiplyBookValue(float bookValueRatio) {
        bookValue *= bookValueRatio;
    }

    /**
     * Get the current book value
     * 
     * @return The current book value
     */
    public float getBookValue() {
        return bookValue;
    }

    /**
     * If this investment is equal to the given investment
     * 
     * @param other The investment to compare this to
     * @return If this investment is equal to the given investment
     */
    @Override
    public boolean equals(Object other) {
        // If the other object is not an investment
        if (!(other instanceof Investment)) {
            return false;
        }
        // Cast the other object to an investment
        Investment otherInvestment = (Investment) other;
        return SYMBOL.equals(otherInvestment.SYMBOL)
                && NAME.equals(otherInvestment.NAME)
                && quantity == otherInvestment.quantity
                && price == otherInvestment.price
                && bookValue == otherInvestment.bookValue;
    }

    /**
     * Returns a string representation of this investment
     * 
     * @return A string representation of this investment
     */
    @Override
    public String toString() {
        // Book value is rounded to two decimal places
        return NAME + " (" + SYMBOL + ")\nPrice: " + price + "\nQuantity: " + quantity + "\nBook Value: "
                + String.format("%.2f", bookValue)
                + "\n";
    }

    /**
     * Buy shares of this investment
     * 
     * @param quantity The number of shares to buy
     * @param price    The current price of the investment
     * @throws Exception If a part of this transaction is invalid
     */
    public abstract void buy(int quantity, float price) throws Exception;

    /**
     * Sell shares of this investment
     * 
     * @param quantity The number of shares to sell
     * @param price    The current price of the investment
     * @return The payment from selling shares
     * @throws Exception If a part of this transaction is invalid
     */
    public abstract float sell(int quantity, float price) throws Exception;

    /**
     * Get the current theoretical gain of this investment if it were to be sold now
     * 
     * @return The current gain of this investment
     */
    public abstract float getGain();
}