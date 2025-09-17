Feature: Copy product

    Scenario: Copy product in catalogue
        Given Admin is on a catalogue page displaying all products
        When Admin checks a checkbox next to product image
        And Admin clicks on copy button
        Then Product is duplicated on a catalogue page with disabled status
        And Admin sees confirmation message
