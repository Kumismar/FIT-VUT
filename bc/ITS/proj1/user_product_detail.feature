Feature: Display product detail

    Scenario: Display product detail from product list page
        Given User is on a page displaying products
        When User selects a product by clicking on its name or picture
        Then Product detail page is loaded
