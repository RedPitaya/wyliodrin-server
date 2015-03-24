/**************************************************************************************************
 * XMPP stuff: connect to Wyliodrin XMPP server
 *************************************************************************************************/

#include <strophe.h> /* Strophe XMPP stuff */

#include "../winternals/winternals.h"
#include "../libds/ds.h"
#include "wxmpp.h"
#include "wxmpp_handlers.h"

hashmap_p tags = NULL; /* tags hashmap */

void shells(const char *from, const char *to, int error, xmpp_stanza_t *stanza) {
  wlog("shells(%s, %s, %d, stanza)", from, to, error);
}

int8_t wxmpp_connect(const char *jid, const char *pass) {
  wlog("wxmpp_connect(%s, %s)", jid, pass);

  /* Initialize the Strophe library */
  xmpp_initialize();

  /* Get Strophe logger, context and connection */
  xmpp_log_t *log  = xmpp_get_default_logger(XMPP_LEVEL_DEBUG); /* Strophe logger */
  xmpp_ctx_t *ctx  = xmpp_ctx_new(NULL, log); /* Strophe context */
  if(ctx == NULL) {
    xmpp_shutdown();

    wlog("Return -1 due to NULL Strophe context");
    return -1;
  }
  xmpp_conn_t *conn = xmpp_conn_new(ctx); /* Strophe connection */
  if(conn == NULL) {
    xmpp_ctx_free(ctx);
    xmpp_shutdown();

    wlog("Return -2 due to NULL Strophe connection");
    return -2;
  }

  /* Setup authentication information */
  xmpp_conn_set_jid(conn, jid);
  xmpp_conn_set_pass(conn, pass);

  /* Initiate connection */
  if(xmpp_connect_client(conn, NULL, WXMPP_PORT, wconn_handler, ctx) < 0) {
    xmpp_conn_release(conn);
    xmpp_ctx_free(ctx);
    xmpp_shutdown();

    wlog("Return -3 due to connection error to XMPP server");
    return -3;
  }

  /* Create tags hashmap */
  tags = create_hashmap();

  /* Add shells tag */
  wadd_tag("shells", shells);

  /* Enter the event loop */
  xmpp_run(ctx);

  /* Cleaning */
  xmpp_conn_release(conn);
  xmpp_ctx_free(ctx);
  xmpp_shutdown();
  destroy_hashmap(tags);
  tags = NULL;

  /* Retry to connect */
  wlog("Retry to connect");
  wxmpp_connect(jid, pass);

  wlog("Retun 0 on success");
  return 0;
}

void wadd_tag(char *tag, tag_function f) {
  hashmap_put(tags, tag, &f, sizeof(void *));
}