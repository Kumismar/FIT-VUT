Feature: User Product List

    Scenario Outline: Number of products listed on a page
        Given User is on a page displaying products
        When User selects to list <number> of products
        Then At most <number> products are listed

        Examples:
            | number |
            | 10     |
            | 25     |
            | 50     |
            | 75     |
            | 100    |

    Scenario: Style of product display
        Given User is on a page displaying products in grid
        When User selects to show products in list
        Then Products are shown in a list

