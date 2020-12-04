# procoid module
The procoid module implements openid login functionality for Drupal websites using the Procurios API.

**Installation instructions**
This module has dependencies on the following custom(ized) SP modules:

* [https://github.com/SPnl/openid_connect](https://github.com/SPnl/openid_connect)
* [https://github.com/SPnl/procapi](https://github.com/SPnl/procapi)

The install these modules before installing procoid.

**usage**
First you need to configure the Procurios API in the [procapi module](https://github.com/SPnl/procapi) settings.

Then go to /admin/config/services/openid-connect.
1. Select the Procurios OpenID connect client.
2. Fill the Procurios configuration fields.
3. Select 'Save user claims on every login' when you want to update user info that has changed in Procurios.
4. Set the 'User claims mapping' to map Procurios relation field values to Drupal user account fields.
5. Use 'Function role mapping' to give Procurios relations with specific functions specific Drupal roles.
6. Optionally restrict OpenID login to Procurios relations with the selected roles only. When this is not selected, every Procurios relation that can login to Procurios can login to Drupal.
7. Optionally select a Procurios selection containing all relations that have at least one of the selected functions. When such a selection is not available, it needs to be created in Procurios first. When such a Procurios selection is chosen, there are two ways to import or update all the relations in this selection in Drupal user accounts. These methods can be used in a cron job.
* Wget the path to trigger a import: /procoid/[drupal site cron key]
* Use drush: drush procoid-import-user
