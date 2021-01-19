# procoid module
The procoid module implements openid login functionality for Drupal websites using the Procurios API.

## Installation instructions ##
This module has dependencies on the following custom(ized) SP modules:

* [https://github.com/SPnl/openid_connect](https://github.com/SPnl/openid_connect)
* [https://github.com/SPnl/procapi](https://github.com/SPnl/procapi)

Install and configure these modules before installing procoid.

## usage ##
First you need to configure the Procurios API in the [procapi module](https://github.com/SPnl/procapi) settings. The procoid module needs the Procurios API sp_custom scope. **Let op, dit zijn niet dezelfde credentials als die voor de procoid functionaliteit!**

Then go to /admin/config/services/openid-connect.
1. Select the Procurios OpenID connect client.
2. Fill the Procurios configuration fields.
3. Select 'Save user claims on every login' when you want to update user info that has changed in Procurios.
4. Set the 'User claims mapping' to map Procurios relation field values to Drupal user account fields.
5. Use 'Function role mapping' to give Procurios relations with specific functions specific Drupal roles.
6. Optionally restrict OpenID login to Procurios relations with the selected roles only. When this is not selected, every Procurios relation that can login to Procurios can login to Drupal.
7. To (regularly) import a list of contacts from Procurios optionally select a Procurios selection containing all relations to be imported. That is, the ones that have at least one of the selected functions in step 5. When such a selection is not available, it needs to be created in Procurios first.
8. Optionally select a existing user account field containing  the (already existing) registration number of the contact. When this is done, existing user accounts will be updated when imported from Procurios.

When a Procurios selection is chosen in step 7 there are two ways to import or update all the relations in this selection in Drupal user accounts. Both these methods can be used in a cron job.
1. Visit https://[site domain]/procoid/[drupal site cron key]
2. use drush: drush procurios-import-users.
