# ITS Projekt 1

- **Autor:** Ondřej Koumar (xkouma02)
- **Datum:** 2024-04-06

## Matice pokrytí artefaktů

Čísla testů jednoznačně identifikují scénář v souborech `.feature`.

|Page|1|2|3|4|5|6|7|8|9|10|11|12|13|14|15|16|17|18|19|
|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|
| Product list page - admin |x|x|x|||||||||||||||||
| Checkbox next to product image - admin |x|||x||||||||||||||||
| Copy button - admin |x|||||||||||||||||||
| Product name filtration - admin ||x||||||||||||||||||
| Apply filter button - admin ||x||||||||||||||||||
| Add product button - admin ||||x||||||||||||||||
| Product information form - admin ||||x|||x|||||||||||||
| Save button on add/edit product page - admin ||||x|||x|||||||||||||
| Delete product button - admin |||||x|||||||||||||||
| Edit product button - admin ||||||x||||||||||||||
| Product name filtration - admin ||x||||||||||||||||||
| Product detail page - user ||||||||x||||||x||||||
| Add to cart button - user ||||||||x||||||||||||
| Cart page - user |||||||||x|x||x||||||||
| Remove from cart button - user ||||||||x||||||||||||
| Quantity field in cart - user ||||||||||x||||||||||
| Change currency dropdown - user |||||||||||x|||||||||
| Product grid page - user |||||||||||x|||x|x|x|x|x||
| Checkout page - user ||||||||||||x|x|||||||
| Checkout button on cart page - user ||||||||||||x||||||||
| Checkout form - user |||||||||||||x|||||||
| Confirm order button - user |||||||||||||x|||||||
| Order status page - user |||||||||||||x|||||||
| Product name/picture button - user ||||||||||||||x||||||
| Number of items dropdown - user |||||||||||||||x|||||
| Product list page - user ||||||||||||||||x||||
| Shop home page - user |||||||||||||||||x|x||
| Search bar - user |||||||||||||||||x|||
| Catalogue browsing panel - user ||||||||||||||||||x||
| Sort by dropdown - user |||||||||||||||||||x|



## Matice pokrytí aktivit

| Activities | 1 | 2 | 3 | 4 |5|6|7|8|9|10|11|12|13|14|15|16|17|18|19|
|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|
| Selecting product to copy/delete - admin |x||||x|||||||||||||||
| Copying product - admin |x|||||||||||||||||||
| Filtering products by name - admin ||x||||||||||||||||||
| Adding products - admin ||||x||||||||||||||||
| Deleting products - admin |||||x|||||||||||||||
| Editing products - admin ||||||x||||||||||||||
| Adding product to cart - user ||||||||x||||||||||||
| Removing product from cart - user |||||||||x|||||||||||
| Editing product quantity in cart - user ||||||||||x||||||||||
| Changing displayed currency - user |||||||||||x|||||||||
| Checking out - user ||||||||||||x|x|||||||
| Displaying product detail - user ||||||||||||||x||||||
| Selecting number of displayed products - user |||||||||||||||x|||||
| Selecting style of displaying products - user ||||||||||||||||x||||
| Searching for products with search bar - user |||||||||||||||||x|||
| Searching for products by clicking on catalogue panel - user ||||||||||||||||||x||
| Selecting sorting option - user |||||||||||||||||||x|


## Matice Feature-Test

| Feature file | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | 10 | 11 | 12 | 13 | 14 | 15 | 16 | 17 | 18 | 19 |
|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|
| admin_product_copy.feature | x |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |
| admin_product_filtration.feature |   | x |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |
| admin_product_operations.feature |   |   | x | x  | x | x | x |  |  |  |  |  |  |  |  |  |  |  |
| user_cart.feature |  |  |  |  |  |  |  | x | x | x |  |  |  |  |  |  |  |  |  |
| user_change_currency.feature | |  |  |  |  |  |  |  |  |  | x |  |  |  |  |  |  |  |  |
| user_checkout.feature |  |  |  |  |  |  |  |  |  |  |  | x | x |  |  |  |  |  |  |
| user_product_detail.feature |  |  |  |  |  |  |  |  |  |  |  |  |  | x |  |  |  |  |  |
| user_product_list.feature |  |  |  |  |  |  |  |  |  |  |  |  |  |  | x | x |  |  |  |
| user_product_search.feature | |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  | x | x |  |
| user_product_sort.feature |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  | x |

