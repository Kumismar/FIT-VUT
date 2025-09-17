Feature: Product operations as administrator

    Background:
        Given Admin is on a catalogue page displaying all products

    Scenario: Navigation to add product page
        When Admin clicks on add button
        Then Admin is redirected to add product page

    Scenario: Add product to catalogue
        Given Admin is on add product page
        And Admin fills all necessary product information
        And Admin clicks on save button
        Then Product is added into the catalogue
        And Admin sees confirmation message

    Scenario: Remove product from catalogue
        When Admin checks checkbox next to product image
        And Admin clicks on delete button
        And Admin confirms alert message
        Then Product is removed from catalogue
        And Admin sees confirmation message

    Scenario: Navigate to product edit page
        When Admin clicks on edit button
        Then Admin is redirected to edit product page

    Scenario: Edit product in catalogue
        Given Admin is on a product edit page
        And Admin fills all necessary product information
        And Admin clicks on save button
        Then Product is updated in catalogue
        And Admin sees confirmation message
