Feature: Change currency

    Scenario: Change currency in the top left corner
        Given User is on a page displaying products
        When User selects <currency> currency in a dropdown menu
        Then All the products have their prices written in <currency>
        And Picture on the dropdown menu displays <currency>

        Examples:
            | currency |
            | euros    |
            | pounds   |
            | dollars  |