package ePortfolio;

import javax.swing.BorderFactory;
import javax.swing.Box;
import javax.swing.BoxLayout;
import javax.swing.JButton;
import javax.swing.JComboBox;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JMenu;
import javax.swing.JMenuBar;
import javax.swing.JMenuItem;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTextArea;
import javax.swing.JTextField;

import java.awt.BorderLayout;
import java.awt.Dimension;
import java.awt.FlowLayout;
import java.awt.GridLayout;

/**
 * Visual elements of the portfolio
 */
public class Portfolio extends JFrame {
    /**
     * Border used between components in a grid
     */
    private final int BORDER = 10;
    /**
     * Investment data
     */
    private DataManager dataManager;
    /**
     * Intro to the program
     */
    private JPanel welcomePanel;
    /**
     * Buy a new investment or buy more of an existing investment
     */
    private JPanel buyPanel;
    /**
     * Sell units of an investment
     */
    private JPanel sellPanel;
    /**
     * Update prices of investments
     */
    private JPanel updatePanel;
    /**
     * Get current gain
     */
    private JPanel getGainPanel;
    /**
     * Search for specific investments
     */
    private JPanel searchPanel;

    /**
     * Setup the frame, create and add all panels, and create the menu
     */
    public Portfolio() {
        super("ePortfolio");
        setSize(520, 440);
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        setLayout(new BoxLayout(getContentPane(), BoxLayout.X_AXIS));
        dataManager = new DataManager();
        createWelcomePanel();
        welcomePanel.setVisible(true);
        add(welcomePanel);
        createBuyPanel();
        buyPanel.setVisible(false);
        add(buyPanel);
        createSellPanel();
        sellPanel.setVisible(false);
        add(sellPanel);
        createUpdatePanel();
        updatePanel.setVisible(false);
        add(updatePanel);
        createGetGainPanel();
        getGainPanel.setVisible(false);
        add(getGainPanel);
        createSearchPanel();
        searchPanel.setVisible(false);
        add(searchPanel);
        createMenu();
    }

    /**
     * Create and add a JMenu to navigate the different pages
     */
    private void createMenu() {
        // Set up menu bar
        JMenuBar menuBar = new JMenuBar();
        JMenu menu = new JMenu("Commands");
        // Create buy menu item
        JMenuItem buyMenuItem = new JMenuItem("Buy");
        buyMenuItem.addActionListener(e -> {
            // Set buy to only visible page
            welcomePanel.setVisible(false);
            buyPanel.setVisible(true);
            sellPanel.setVisible(false);
            updatePanel.setVisible(false);
            getGainPanel.setVisible(false);
            searchPanel.setVisible(false);
        });
        menu.add(buyMenuItem);
        // Create sell menu item
        JMenuItem sellMenuItem = new JMenuItem("Sell");
        sellMenuItem.addActionListener(e -> {
            // Set sell to only visible page
            welcomePanel.setVisible(false);
            buyPanel.setVisible(false);
            sellPanel.setVisible(true);
            updatePanel.setVisible(false);
            getGainPanel.setVisible(false);
            searchPanel.setVisible(false);
        });
        menu.add(sellMenuItem);
        // Create update menu item
        JMenuItem updateMenuItem = new JMenuItem("Update");
        updateMenuItem.addActionListener(e -> {
            // Set update to only visible page
            welcomePanel.setVisible(false);
            buyPanel.setVisible(false);
            sellPanel.setVisible(false);
            updatePanel.setVisible(true);
            getGainPanel.setVisible(false);
            searchPanel.setVisible(false);
            updatePosition = 0;
            // If there are no investments
            if (dataManager.getAllInvestments().size() == 0) {
                updateSymbolTextField.setText("");
                updateNameTextField.setText("");
                updatePrevButton.setEnabled(false);
                updateNextButton.setEnabled(false);
                // If there is only one investment
            } else if (dataManager.getAllInvestments().size() == 1) {
                updateSymbolTextField.setText(dataManager.getAllInvestments().get(0).SYMBOL);
                updateNameTextField.setText(dataManager.getAllInvestments().get(0).NAME);
                updatePrevButton.setEnabled(false);
                updateNextButton.setEnabled(false);
                // If there are more than one investments
            } else {
                updateSymbolTextField.setText(dataManager.getAllInvestments().get(0).SYMBOL);
                updateNameTextField.setText(dataManager.getAllInvestments().get(0).NAME);
                updatePrevButton.setEnabled(false);
                updateNextButton.setEnabled(true);
            }
        });
        menu.add(updateMenuItem);
        // Create getGain menu item
        JMenuItem getGainMenuItem = new JMenuItem("Get Gain");
        getGainMenuItem.addActionListener(e -> {
            // Set getGain to only visible page
            welcomePanel.setVisible(false);
            buyPanel.setVisible(false);
            sellPanel.setVisible(false);
            updatePanel.setVisible(false);
            getGainPanel.setVisible(true);
            searchPanel.setVisible(false);
            // Update current gain
            totalGainTextField.setText(dataManager.getTotalGain());
            individualGainsTextArea.setText(dataManager.getIndividualGains());
        });
        menu.add(getGainMenuItem);
        // Create search menu item
        JMenuItem searchMenuItem = new JMenuItem("Search");
        searchMenuItem.addActionListener(e -> {
            // Set search to only visible page
            welcomePanel.setVisible(false);
            buyPanel.setVisible(false);
            sellPanel.setVisible(false);
            updatePanel.setVisible(false);
            getGainPanel.setVisible(false);
            searchPanel.setVisible(true);
        });
        menu.add(searchMenuItem);
        // Create quit menu item
        JMenuItem quitMenuItem = new JMenuItem("Quit");
        quitMenuItem.addActionListener(e -> {
            // Exit program
            System.exit(0);
        });
        // Add menu
        menu.add(quitMenuItem);
        menuBar.add(menu);
        setJMenuBar(menuBar);
    }

    /**
     * Create an panel with intro information for the program
     */
    private void createWelcomePanel() {
        welcomePanel = new JPanel(new GridLayout(3, 1));
        // Panel for the intro title
        JPanel welcomeTitlePanel = new JPanel(new FlowLayout(FlowLayout.LEFT));
        // Format panel
        welcomeTitlePanel.setBorder(BorderFactory.createEmptyBorder(80, 20, 20, 20));
        welcomeTitlePanel.add(new JLabel("Welcome to ePortfolio"));
        welcomePanel.add(welcomeTitlePanel);
        // Panel for the intro paragraph
        JPanel welcomeParagraphPanel = new JPanel();
        welcomeParagraphPanel.setLayout(new BoxLayout(welcomeParagraphPanel, BoxLayout.Y_AXIS));
        // Format panel
        welcomeParagraphPanel.setBorder(BorderFactory.createEmptyBorder(80, 20, 0, 20));
        // Intro paragraph
        welcomeParagraphPanel.add(new JLabel("Choose a command from the \"Commands\" menu to buy or sell"));
        welcomeParagraphPanel.add(new JLabel("an investment, update prices for all investments, get gain for the"));
        welcomeParagraphPanel.add(new JLabel("portfolio, search for relevant investments, or quit the program"));
        welcomePanel.add(welcomeParagraphPanel);
        // Fill blank space
        welcomePanel.add(new JPanel());
    }

    /**
     * Create a panel to buy a new investment
     */
    public void createBuyPanel() {
        // Create buy panel
        buyPanel = new JPanel(new BorderLayout());
        // Components where the user can enter information
        JPanel optionsPanel = new JPanel(new BorderLayout());
        JLabel titleLabel = new JLabel("Buying an investment");
        titleLabel.setBorder(BorderFactory.createEmptyBorder(15, 15, 15, 0));
        optionsPanel.add(titleLabel, BorderLayout.NORTH);
        // Labels for the input components
        JPanel labelPanel = new JPanel(new GridLayout(5, 1, BORDER, BORDER));
        labelPanel.setBorder(BorderFactory.createEmptyBorder(0, 40, 0, BORDER));
        // Adding the labels wrapped in panels
        JPanel typeLabelPanel = new JPanel(new FlowLayout(FlowLayout.LEFT));
        typeLabelPanel.add(new JLabel("Type"));
        labelPanel.add(typeLabelPanel);
        JPanel symbolLabelPanel = new JPanel(new FlowLayout(FlowLayout.LEFT));
        symbolLabelPanel.add(new JLabel("Symbol"));
        labelPanel.add(symbolLabelPanel);
        JPanel nameLabelPanel = new JPanel(new FlowLayout(FlowLayout.LEFT));
        nameLabelPanel.add(new JLabel("Name"));
        labelPanel.add(nameLabelPanel);
        JPanel quantityLabelPanel = new JPanel(new FlowLayout(FlowLayout.LEFT));
        quantityLabelPanel.add(new JLabel("Quantity"));
        labelPanel.add(quantityLabelPanel);
        JPanel priceLabelPanel = new JPanel(new FlowLayout(FlowLayout.LEFT));
        priceLabelPanel.add(new JLabel("Price"));
        labelPanel.add(priceLabelPanel);
        optionsPanel.add(labelPanel, BorderLayout.WEST);
        // Text fields for the investment to buy
        JPanel inputPanel = new JPanel(new GridLayout(5, 1, BORDER, BORDER));
        inputPanel.setBorder(BorderFactory.createEmptyBorder(0, 0, 0, BORDER));
        // Type of the investment to buy
        JPanel typePanel = new JPanel(new FlowLayout(FlowLayout.LEFT));
        String[] typeOptions = { "Stock", "Mutual Fund" };
        JComboBox<String> typeComboBox = new JComboBox<>(typeOptions);
        typePanel.add(typeComboBox);
        inputPanel.add(typePanel);
        // Symbol of the investment to buy
        JPanel symbolPanel = new JPanel(new FlowLayout(FlowLayout.LEFT));
        JTextField symbolTextField = new JTextField(12);
        symbolPanel.add(symbolTextField);
        inputPanel.add(symbolPanel);
        // Name of the investment to buy
        JPanel namePanel = new JPanel(new FlowLayout(FlowLayout.LEFT));
        JTextField nameTextField = new JTextField(20);
        namePanel.add(nameTextField);
        inputPanel.add(namePanel);
        // Quantity of the investment to buy
        JPanel quantityPanel = new JPanel(new FlowLayout(FlowLayout.LEFT));
        JTextField quantityTextField = new JTextField(8);
        quantityPanel.add(quantityTextField);
        inputPanel.add(quantityPanel);
        // Price of the investment to buy
        JPanel pricePanel = new JPanel(new FlowLayout(FlowLayout.LEFT));
        JTextField priceTextField = new JTextField(8);
        pricePanel.add(priceTextField);
        inputPanel.add(pricePanel);
        optionsPanel.add(inputPanel, BorderLayout.CENTER);
        buyPanel.add(optionsPanel, BorderLayout.CENTER);
        // Input buttons
        JPanel buttonPanel = new JPanel();
        buttonPanel.setLayout(new BoxLayout(buttonPanel, BoxLayout.Y_AXIS));
        buttonPanel.setPreferredSize(new Dimension(150, 0));
        buttonPanel.add(Box.createVerticalGlue());
        // Button for resetting information
        JPanel resetButtonPanel = new JPanel();
        JButton resetButton = new JButton("Reset");
        resetButton.addActionListener(e -> {
            // Clear all options
            typeComboBox.setSelectedItem(typeOptions[0]);
            symbolTextField.setText("");
            nameTextField.setText("");
            quantityTextField.setText("");
            priceTextField.setText("");
        });
        resetButtonPanel.add(resetButton);
        buttonPanel.add(resetButtonPanel);
        // Button for buying an investment
        JPanel buyButtonPanel = new JPanel();
        JButton buyButton = new JButton("Buy");
        JTextArea messagesTextArea = new JTextArea(6, 40);
        buyButton.addActionListener(e -> {
            // Get user input
            String type = (String) typeComboBox.getSelectedItem();
            String symbol = symbolTextField.getText().toUpperCase();
            String name = nameTextField.getText();
            String quantity = quantityTextField.getText();
            String price = priceTextField.getText();
            // Buy investment and print output
            try {
                messagesTextArea.append(dataManager.buyInvestment(type, symbol, name, quantity, price) + "\n");
            } catch (Exception exception) {
                // Print any error messages
                messagesTextArea.append("ERROR: " + exception.getMessage() + "\n");
            }
        });
        buyButtonPanel.add(buyButton);
        buttonPanel.add(buyButtonPanel);
        buyPanel.add(buttonPanel, BorderLayout.EAST);
        // Output messages
        JPanel messagesPanel = new JPanel(new BorderLayout());
        // Messages panel header
        JPanel messagesLabelPanel = new JPanel(new FlowLayout(FlowLayout.LEFT));
        JLabel messagesLabel = new JLabel("Messages");
        messagesLabel.setBorder(BorderFactory.createEmptyBorder(0, BORDER, 0, 0));
        messagesLabelPanel.add(messagesLabel);
        messagesPanel.add(messagesLabelPanel, BorderLayout.NORTH);
        // Messages panel text field
        JPanel messagesTextPanel = new JPanel();
        messagesTextPanel.setBorder(BorderFactory.createEmptyBorder(0, BORDER, BORDER, BORDER));
        JScrollPane messagesScrollPane = new JScrollPane(messagesTextArea);
        messagesScrollPane.setVerticalScrollBarPolicy(JScrollPane.VERTICAL_SCROLLBAR_ALWAYS);
        messagesScrollPane.setHorizontalScrollBarPolicy(JScrollPane.HORIZONTAL_SCROLLBAR_ALWAYS);
        messagesTextPanel.add(messagesScrollPane);
        messagesPanel.add(messagesTextPanel, BorderLayout.CENTER);
        buyPanel.add(messagesPanel, BorderLayout.SOUTH);
    }

    /**
     * Create a panel to sell an existing investment
     */
    private void createSellPanel() {
        // Create sell panel
        sellPanel = new JPanel(new BorderLayout());
        // Components where the user can enter information
        JPanel optionsPanel = new JPanel(new BorderLayout());
        JLabel titleLabel = new JLabel("Selling an investment");
        titleLabel.setBorder(BorderFactory.createEmptyBorder(15, 15, 15, 0));
        optionsPanel.add(titleLabel, BorderLayout.NORTH);
        // Labels for the input components
        JPanel labelPanel = new JPanel(new GridLayout(3, 1, BORDER, BORDER));
        labelPanel.setBorder(BorderFactory.createEmptyBorder(0, 40, 0, BORDER));
        // Adding the labels wrapped in panels
        JPanel symbolLabelPanel = new JPanel(new FlowLayout(FlowLayout.LEFT));
        symbolLabelPanel.add(new JLabel("Symbol"));
        labelPanel.add(symbolLabelPanel);
        JPanel quantityLabelPanel = new JPanel(new FlowLayout(FlowLayout.LEFT));
        quantityLabelPanel.add(new JLabel("Quantity"));
        labelPanel.add(quantityLabelPanel);
        JPanel priceLabelPanel = new JPanel(new FlowLayout(FlowLayout.LEFT));
        priceLabelPanel.add(new JLabel("Price"));
        labelPanel.add(priceLabelPanel);
        optionsPanel.add(labelPanel, BorderLayout.WEST);
        // Text fields for the investment to sell
        JPanel inputPanel = new JPanel(new GridLayout(3, 1, BORDER, BORDER));
        inputPanel.setBorder(BorderFactory.createEmptyBorder(0, 0, 0, BORDER));
        // Symbol of the investment to sell
        JPanel symbolPanel = new JPanel(new FlowLayout(FlowLayout.LEFT));
        JTextField symbolTextField = new JTextField(12);
        symbolPanel.add(symbolTextField);
        inputPanel.add(symbolPanel);
        // Quantity of the investment to sell
        JPanel quantityPanel = new JPanel(new FlowLayout(FlowLayout.LEFT));
        JTextField quantityTextField = new JTextField(8);
        quantityPanel.add(quantityTextField);
        inputPanel.add(quantityPanel);
        // Updated price of the investment to sell
        JPanel pricePanel = new JPanel(new FlowLayout(FlowLayout.LEFT));
        JTextField priceTextField = new JTextField(8);
        pricePanel.add(priceTextField);
        inputPanel.add(pricePanel);
        optionsPanel.add(inputPanel, BorderLayout.CENTER);
        sellPanel.add(optionsPanel, BorderLayout.CENTER);
        // Input button
        JPanel buttonPanel = new JPanel();
        buttonPanel.setLayout(new BoxLayout(buttonPanel, BoxLayout.Y_AXIS));
        buttonPanel.setPreferredSize(new Dimension(150, 0));
        buttonPanel.add(Box.createVerticalGlue());
        // Button for resetting information
        JPanel resetButtonPanel = new JPanel();
        JButton resetButton = new JButton("Reset");
        resetButton.addActionListener(e -> {
            // Clear all options
            symbolTextField.setText("");
            quantityTextField.setText("");
            priceTextField.setText("");
        });
        resetButtonPanel.add(resetButton);
        buttonPanel.add(resetButtonPanel);
        // Button for selling an investment
        JPanel sellButtonPanel = new JPanel();
        JButton sellButton = new JButton("Sell");
        JTextArea messagesTextArea = new JTextArea(6, 40);
        sellButton.addActionListener(e -> {
            // Get user input
            String symbol = symbolTextField.getText().toUpperCase();
            String quantity = quantityTextField.getText();
            String price = priceTextField.getText();
            // Sell investment and print output
            try {
                messagesTextArea.append(dataManager.sellInvestment(symbol, quantity, price) + "\n");
            } catch (Exception exception) {
                // Print any error messages
                messagesTextArea.append("ERROR: " + exception.getMessage() + "\n");
            }
        });
        sellButtonPanel.add(sellButton);
        buttonPanel.add(sellButtonPanel);
        sellPanel.add(buttonPanel, BorderLayout.EAST);
        // Output messages
        JPanel messagesPanel = new JPanel(new BorderLayout());
        // Messages panel header
        JPanel messagesLabelPanel = new JPanel(new FlowLayout(FlowLayout.LEFT));
        JLabel messagesLabel = new JLabel("Messages");
        messagesLabel.setBorder(BorderFactory.createEmptyBorder(0, BORDER, 0, 0));
        messagesLabelPanel.add(messagesLabel);
        messagesPanel.add(messagesLabelPanel, BorderLayout.NORTH);
        // Messages panel text fields
        JPanel messagesTextPanel = new JPanel();
        messagesTextPanel.setBorder(BorderFactory.createEmptyBorder(0, BORDER, BORDER, BORDER));
        JScrollPane messagesScrollPane = new JScrollPane(messagesTextArea);
        messagesScrollPane.setVerticalScrollBarPolicy(JScrollPane.VERTICAL_SCROLLBAR_ALWAYS);
        messagesScrollPane.setHorizontalScrollBarPolicy(JScrollPane.HORIZONTAL_SCROLLBAR_ALWAYS);
        messagesTextPanel.add(messagesScrollPane);
        messagesPanel.add(messagesTextPanel, BorderLayout.CENTER);
        sellPanel.add(messagesPanel, BorderLayout.SOUTH);
    }

    /**
     * Current investment to update
     */
    private int updatePosition;
    /**
     * Symbol of current investment to update
     */
    private JTextField updateSymbolTextField;
    /**
     * Name of current investment to update
     */
    private JTextField updateNameTextField;
    /**
     * Move to previous investment
     */
    private JButton updatePrevButton;
    /**
     * Move to next investment
     */
    private JButton updateNextButton;

    /**
     * Create a panel for updating the prices of investments
     */
    public void createUpdatePanel() {
        // Create update panel
        updatePanel = new JPanel(new BorderLayout());
        // Components where the user can enter information
        JPanel optionsPanel = new JPanel(new BorderLayout());
        JLabel titleLabel = new JLabel("Updating investments");
        titleLabel.setBorder(BorderFactory.createEmptyBorder(15, 15, 15, 0));
        optionsPanel.add(titleLabel, BorderLayout.NORTH);
        // Labels for the input components
        JPanel labelPanel = new JPanel(new GridLayout(3, 1, BORDER, BORDER));
        labelPanel.setBorder(BorderFactory.createEmptyBorder(0, 40, 0, BORDER));
        // Adding the labels wrapped in panels
        JPanel symbolLabelPanel = new JPanel(new FlowLayout(FlowLayout.LEFT));
        symbolLabelPanel.add(new JLabel("Symbol"));
        labelPanel.add(symbolLabelPanel);
        JPanel nameLabelPanel = new JPanel(new FlowLayout(FlowLayout.LEFT));
        nameLabelPanel.add(new JLabel("Name"));
        labelPanel.add(nameLabelPanel);
        JPanel priceLabelPanel = new JPanel(new FlowLayout(FlowLayout.LEFT));
        priceLabelPanel.add(new JLabel("Price"));
        labelPanel.add(priceLabelPanel);
        optionsPanel.add(labelPanel, BorderLayout.WEST);
        // Text fields for the investment to update
        JPanel inputPanel = new JPanel(new GridLayout(3, 1, BORDER, BORDER));
        inputPanel.setBorder(BorderFactory.createEmptyBorder(0, 0, 0, BORDER));
        // Symbol of the investment to update
        JPanel symbolPanel = new JPanel(new FlowLayout(FlowLayout.LEFT));
        updateSymbolTextField = new JTextField(12);
        updateSymbolTextField.setEditable(false);
        symbolPanel.add(updateSymbolTextField);
        inputPanel.add(symbolPanel);
        // Name of the investment to update
        JPanel namePanel = new JPanel(new FlowLayout(FlowLayout.LEFT));
        updateNameTextField = new JTextField(20);
        updateNameTextField.setEditable(false);
        namePanel.add(updateNameTextField);
        inputPanel.add(namePanel);
        // Updates price of the investment
        JPanel pricePanel = new JPanel(new FlowLayout(FlowLayout.LEFT));
        JTextField priceTextField = new JTextField(8);
        pricePanel.add(priceTextField);
        inputPanel.add(pricePanel);
        optionsPanel.add(inputPanel, BorderLayout.CENTER);
        updatePanel.add(optionsPanel, BorderLayout.CENTER);
        // Input buttons
        JPanel buttonPanel = new JPanel();
        buttonPanel.setLayout(new BoxLayout(buttonPanel, BoxLayout.Y_AXIS));
        buttonPanel.setPreferredSize(new Dimension(150, 0));
        buttonPanel.add(Box.createVerticalGlue());
        // Button for the previous investment
        JPanel prevButtonPanel = new JPanel();
        updatePrevButton = new JButton("Prev");
        updateNextButton = new JButton("Next");
        updatePrevButton.addActionListener(e -> {
            // Enable next button if needed
            if (updatePosition == dataManager.getAllInvestments().size() - 1) {
                updateNextButton.setEnabled(true);
            }
            // Move position
            updatePosition--;
            // Disable previous button if needed
            if (updatePosition == 0) {
                updatePrevButton.setEnabled(false);
            }
            // Update information
            updateSymbolTextField.setText(dataManager.getAllInvestments().get(updatePosition).SYMBOL);
            updateNameTextField.setText(dataManager.getAllInvestments().get(updatePosition).NAME);
        });
        prevButtonPanel.add(updatePrevButton);
        buttonPanel.add(prevButtonPanel);
        // Button for the next investment
        JPanel nextButtonPanel = new JPanel();
        updateNextButton.addActionListener(e -> {
            // Enable previous button if needed
            if (updatePosition == 0) {
                updatePrevButton.setEnabled(true);
            }
            // Move position
            updatePosition++;
            // Disable next button if needed
            if (updatePosition == dataManager.getAllInvestments().size() - 1) {
                updateNextButton.setEnabled(false);
            }
            // Update information
            updateSymbolTextField.setText(dataManager.getAllInvestments().get(updatePosition).SYMBOL);
            updateNameTextField.setText(dataManager.getAllInvestments().get(updatePosition).NAME);
        });
        nextButtonPanel.add(updateNextButton);
        buttonPanel.add(nextButtonPanel);
        // Button to save the new price
        JPanel saveButtonPanel = new JPanel();
        JButton saveButton = new JButton("Save");
        JTextArea messagesTextArea = new JTextArea(6, 40);
        saveButton.addActionListener(e -> {
            // If there is an investment in the system
            if (dataManager.getAllInvestments().size() != 0) {
                float price = -1;
                // Try converting price to a float
                try {
                    price = Float.parseFloat(priceTextField.getText());
                } catch (Exception exception) {
                    messagesTextArea.append("ERROR: Price must be a number.\n");
                    return;
                }
                // Updating the price and printing output
                try {
                    dataManager.getAllInvestments().get(updatePosition).setPrice(price);
                    messagesTextArea
                            .append("Updated the price of " + dataManager.getAllInvestments().get(updatePosition).NAME
                                    + " (" + dataManager.getAllInvestments().get(updatePosition).SYMBOL + ") to "
                                    + String.format("%.2f", price) + "\n");
                } catch (Exception exception) {
                    // Print any error messages
                    messagesTextArea.append("ERROR: " + exception.getMessage() + "\n");
                }
            }
        });
        saveButtonPanel.add(saveButton);
        buttonPanel.add(saveButtonPanel);
        updatePanel.add(buttonPanel, BorderLayout.EAST);
        // Output messages
        JPanel messagesPanel = new JPanel(new BorderLayout());
        // Messages panel header
        JPanel messagesLabelPanel = new JPanel(new FlowLayout(FlowLayout.LEFT));
        JLabel messagesLabel = new JLabel("Messages");
        messagesLabel.setBorder(BorderFactory.createEmptyBorder(0, BORDER, 0, 0));
        messagesLabelPanel.add(messagesLabel);
        messagesPanel.add(messagesLabelPanel, BorderLayout.NORTH);
        // Messages panel text field
        JPanel messagesTextPanel = new JPanel();
        messagesTextPanel.setBorder(BorderFactory.createEmptyBorder(0, BORDER, BORDER, BORDER));
        JScrollPane messagesScrollPane = new JScrollPane(messagesTextArea);
        messagesScrollPane.setVerticalScrollBarPolicy(JScrollPane.VERTICAL_SCROLLBAR_ALWAYS);
        messagesScrollPane.setHorizontalScrollBarPolicy(JScrollPane.HORIZONTAL_SCROLLBAR_ALWAYS);
        messagesTextPanel.add(messagesScrollPane);
        messagesPanel.add(messagesTextPanel, BorderLayout.CENTER);
        updatePanel.add(messagesPanel, BorderLayout.SOUTH);
    }

    /**
     * Output for total gain
     */
    private JTextField totalGainTextField;
    /**
     * Output for individual gains
     */
    private JTextArea individualGainsTextArea;

    /**
     * Create a panel for calculating the total gain of all investments
     */
    public void createGetGainPanel() {
        // Create get gain panel
        getGainPanel = new JPanel(new BorderLayout());
        // Components where the user can enter information
        JPanel optionsPanel = new JPanel(new BorderLayout());
        JLabel titleLabel = new JLabel("Getting total gain");
        titleLabel.setBorder(BorderFactory.createEmptyBorder(15, 15, 15, 0));
        optionsPanel.add(titleLabel, BorderLayout.NORTH);
        // Labels for the input components
        JPanel labelPanel = new JPanel(new GridLayout(1, 1, BORDER, BORDER));
        labelPanel.setBorder(BorderFactory.createEmptyBorder(0, 40, 0, BORDER));
        // Label for the total gain
        JPanel totalGainLabelPanel = new JPanel(new FlowLayout(FlowLayout.LEFT));
        totalGainLabelPanel.add(new JLabel("Total gain"));
        labelPanel.add(totalGainLabelPanel);
        optionsPanel.add(labelPanel, BorderLayout.WEST);
        // Text fields for the total gain
        JPanel inputPanel = new JPanel(new GridLayout(1, 1, BORDER, BORDER));
        inputPanel.setBorder(BorderFactory.createEmptyBorder(0, 0, 0, BORDER));
        // Total gain of all investments
        JPanel totalGainPanel = new JPanel(new FlowLayout(FlowLayout.LEFT));
        totalGainTextField = new JTextField(12);
        totalGainTextField.setEditable(false);
        totalGainPanel.add(totalGainTextField);
        inputPanel.add(totalGainPanel);
        optionsPanel.add(inputPanel, BorderLayout.CENTER);
        getGainPanel.add(optionsPanel, BorderLayout.CENTER);
        // Output messages
        JPanel messagesPanel = new JPanel(new BorderLayout());
        // Messages panel header
        JPanel messagesLabelPanel = new JPanel(new FlowLayout(FlowLayout.LEFT));
        JLabel messagesLabel = new JLabel("Individual gains");
        messagesLabel.setBorder(BorderFactory.createEmptyBorder(0, BORDER, 0, 0));
        messagesLabelPanel.add(messagesLabel);
        messagesPanel.add(messagesLabelPanel, BorderLayout.NORTH);
        // Text fields for individual gains
        JPanel messagesTextPanel = new JPanel();
        messagesTextPanel.setBorder(BorderFactory.createEmptyBorder(0, BORDER, BORDER, BORDER));
        individualGainsTextArea = new JTextArea(12, 40);
        JScrollPane messagesScrollPane = new JScrollPane(individualGainsTextArea);
        messagesScrollPane.setVerticalScrollBarPolicy(JScrollPane.VERTICAL_SCROLLBAR_ALWAYS);
        messagesScrollPane.setHorizontalScrollBarPolicy(JScrollPane.HORIZONTAL_SCROLLBAR_ALWAYS);
        messagesTextPanel.add(messagesScrollPane);
        messagesPanel.add(messagesTextPanel, BorderLayout.CENTER);
        getGainPanel.add(messagesPanel, BorderLayout.SOUTH);
    }

    /**
     * Create a panel to search for specific investments
     */
    public void createSearchPanel() {
        // Create search panel
        searchPanel = new JPanel(new BorderLayout());
        // Components where the user can enter information
        JPanel optionsPanel = new JPanel(new BorderLayout());
        JLabel titleLabel = new JLabel("Searching investments");
        titleLabel.setBorder(BorderFactory.createEmptyBorder(15, 15, 15, 0));
        optionsPanel.add(titleLabel, BorderLayout.NORTH);
        // Labels for the input components
        JPanel labelPanel = new JPanel(new GridLayout(4, 1, BORDER, BORDER));
        labelPanel.setBorder(BorderFactory.createEmptyBorder(0, 40, 0, BORDER));
        // Adding the labels wrapped in panels
        JPanel symbolLabelPanel = new JPanel(new FlowLayout(FlowLayout.LEFT));
        symbolLabelPanel.add(new JLabel("Symbol"));
        labelPanel.add(symbolLabelPanel);
        JPanel nameKeywordsLabelPanel = new JPanel(new FlowLayout(FlowLayout.LEFT));
        nameKeywordsLabelPanel.add(new JLabel("Name keywords"));
        labelPanel.add(nameKeywordsLabelPanel);
        JPanel lowPriceLabelPanel = new JPanel(new FlowLayout(FlowLayout.LEFT));
        lowPriceLabelPanel.add(new JLabel("Low price"));
        labelPanel.add(lowPriceLabelPanel);
        JPanel highPriceLabelPanel = new JPanel(new FlowLayout(FlowLayout.LEFT));
        highPriceLabelPanel.add(new JLabel("High price"));
        labelPanel.add(highPriceLabelPanel);
        optionsPanel.add(labelPanel, BorderLayout.WEST);
        // Text fields for the investment to buy
        JPanel inputPanel = new JPanel(new GridLayout(4, 1, BORDER, BORDER));
        inputPanel.setBorder(BorderFactory.createEmptyBorder(0, 0, 0, BORDER));
        // Symbol of the investments to search for
        JPanel symbolPanel = new JPanel(new FlowLayout(FlowLayout.LEFT));
        JTextField symbolTextField = new JTextField(12);
        symbolPanel.add(symbolTextField);
        inputPanel.add(symbolPanel);
        // Keywords of in the name of the investments to search for
        JPanel nameKeywordsPanel = new JPanel(new FlowLayout(FlowLayout.LEFT));
        JTextField nameKeywordsTextField = new JTextField(20);
        nameKeywordsPanel.add(nameKeywordsTextField);
        inputPanel.add(nameKeywordsPanel);
        // Lowest price of the investments to search for
        JPanel lowPricePanel = new JPanel(new FlowLayout(FlowLayout.LEFT));
        JTextField lowPriceTextField = new JTextField(8);
        lowPricePanel.add(lowPriceTextField);
        inputPanel.add(lowPricePanel);
        // Highest price of the investments to search for
        JPanel highPricePanel = new JPanel(new FlowLayout(FlowLayout.LEFT));
        JTextField highPriceTextField = new JTextField(8);
        highPricePanel.add(highPriceTextField);
        inputPanel.add(highPricePanel);
        optionsPanel.add(inputPanel, BorderLayout.CENTER);
        searchPanel.add(optionsPanel, BorderLayout.CENTER);
        // Input buttons
        JPanel buttonPanel = new JPanel();
        buttonPanel.setLayout(new BoxLayout(buttonPanel, BoxLayout.Y_AXIS));
        buttonPanel.setPreferredSize(new Dimension(150, 0));
        buttonPanel.add(Box.createVerticalGlue());
        // Button for resetting information
        JPanel resetButtonPanel = new JPanel();
        JButton resetButton = new JButton("Reset");
        resetButton.addActionListener(e -> {
            // Clear all options
            symbolTextField.setText("");
            nameKeywordsTextField.setText("");
            lowPriceTextField.setText("");
            highPriceTextField.setText("");
        });
        resetButtonPanel.add(resetButton);
        buttonPanel.add(resetButtonPanel);
        // Button for searching for investments
        JPanel searchButtonPanel = new JPanel();
        JButton searchButton = new JButton("Search");
        JTextArea messagesTextArea = new JTextArea(6, 40);
        searchButton.addActionListener(e -> {
            // Get user input
            String symbol = symbolTextField.getText().toUpperCase();
            String nameKeywords = nameKeywordsTextField.getText();
            String lowPrice = lowPriceTextField.getText();
            String highPrice = highPriceTextField.getText();
            // Search for investments and print output
            try {
                messagesTextArea.append(dataManager.search(symbol, nameKeywords, lowPrice, highPrice) + "\n");
            } catch (Exception exception) {
                // Print any error messages
                messagesTextArea.append("ERROR: " + exception.getMessage() + "\n");
            }
        });
        searchButtonPanel.add(searchButton);
        buttonPanel.add(searchButtonPanel);
        searchPanel.add(buttonPanel, BorderLayout.EAST);
        // Output messages
        JPanel messagesPanel = new JPanel(new BorderLayout());
        // Messages panel header
        JPanel messagesLabelPanel = new JPanel(new FlowLayout(FlowLayout.LEFT));
        JLabel messagesLabel = new JLabel("Search results");
        messagesLabel.setBorder(BorderFactory.createEmptyBorder(0, BORDER, 0, 0));
        messagesLabelPanel.add(messagesLabel);
        messagesPanel.add(messagesLabelPanel, BorderLayout.NORTH);
        // Text fields for the search results
        JPanel messagesTextPanel = new JPanel();
        messagesTextPanel.setBorder(BorderFactory.createEmptyBorder(0, BORDER, BORDER, BORDER));
        JScrollPane messagesScrollPane = new JScrollPane(messagesTextArea);
        messagesScrollPane.setVerticalScrollBarPolicy(JScrollPane.VERTICAL_SCROLLBAR_ALWAYS);
        messagesScrollPane.setHorizontalScrollBarPolicy(JScrollPane.HORIZONTAL_SCROLLBAR_ALWAYS);
        messagesTextPanel.add(messagesScrollPane);
        messagesPanel.add(messagesTextPanel, BorderLayout.CENTER);
        searchPanel.add(messagesPanel, BorderLayout.SOUTH);
    }

    /**
     * This program stores stocks and mutualfunds in a database, and provides a
     * visual GUI for viewing and maintaining the data
     * 
     * @param args Command line arguments
     */
    public static void main(String[] args) {
        Portfolio main = new Portfolio();
        main.setVisible(true);
    }
}
