Feature: Cart functionality

    Scenario: Adding product to cart from product detail page
        Given User is on a product detail page
        When User clicks on add to cart button
        Then Product appears in user cart
        And User sees confirmation message

    Scenario: Removing product from cart
        Given User is on a page displaying cart
        When User clicks on remove from cart button
        Then Product is removed from cart
        And User sees confirmation message

    Scenario: Editing product quantity in cart
        Given User is on a page displaying cart
        When User fills out quantity field
        And User clicks on update button
        Then Product quantity is updated in cart
        And User sees confirmation message
