Feature: User Product Search

    Scenario: Product search with search bar
        Given User is at the shop home page
        When User enters "canon" into search bar
        Then Products related to "canon" are shown on the page displaying products grid

    Scenario: Product search with catalogue
        Given User is at the shop home page
        When User selects "Cameras" in the catalogue
        Then "Cameras" are shown on the page displaying products grid