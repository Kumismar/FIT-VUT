Feature: Product Filtration For Admin

    Scenario: Admin filters products
        Given Admin is on a catalogue page displaying all products
        When Admin types <product_name> in Product Name
        And Admin confirms filter selection
        Then Only products named <product_name> are shown

        Examples:
            | product_name |
            | iMac         |
            | iPhone       |
            | MacBook Air  |
            | MacBook Pro  |