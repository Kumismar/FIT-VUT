Feature: Checkout functionality

    Scenario: Navigation from cart to checkout
        Given User is on a page displaying cart
        When User clicks checkout button
        Then User is redirected to checkout page

    Scenario: Order confirmation
        Given User is on a checkout page
        When User fills out all necessary data
        And User clicks on confirm order
        Then Order status page appears