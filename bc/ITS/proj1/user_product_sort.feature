Feature: User Product Sort

    Scenario Outline: Various Product Sorts
        Given User is on a page displaying products grid
        When User applies sorting option <option>
        Then Products are sorted by <option>

        Examples:
            | option           |
            | Name (A-Z)       |
            | Name (Z-A)       |
            | Price (Low-High) |
            | Price (High-Low) |
            | Rating (Highest) |
            | Rating (Lowest)  |
            | Model (A-Z)      |
            | Model (Z-A)      |