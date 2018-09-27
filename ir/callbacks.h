#ifndef _CALLBACKS_H
#define _CALLBACKS_H

constexpr static int CB_SUCCESS = 0;
constexpr static int CB_FAILED = 1;

struct sd_callbacks;
//--------------------------------------------------------------------
// function declarations of struct sd_callbacks
//--------------------------------------------------------------------
/* block level callbacks - NULL skips the block */
void blockcode(struct buf *ob, const struct buf *text, const struct buf *lang,
               void *opaque);
void blockquote(struct buf *ob, const struct buf *text, void *opaque);
void blockhtml(struct buf *ob, const struct buf *text, void *opaque);
void header(struct buf *ob, const struct buf *text, int level, void *opaque);
void hrule(struct buf *ob, void *opaque);
void list(struct buf *ob, const struct buf *text, int flags, void *opaque);
void listitem(struct buf *ob, const struct buf *text, int flags, void *opaque);
void paragraph(struct buf *ob, const struct buf *text, void *opaque);
void table(struct buf *ob, const struct buf *header, const struct buf *body,
           void *opaque);
void table_row(struct buf *ob, const struct buf *text, void *opaque);
void table_cell(struct buf *ob, const struct buf *text, int flags,
                void *opaque);

/* span level callbacks - NULL or return 0 prints the span verbatim */
int autolink(struct buf *ob, const struct buf *link, enum mkd_autolink type,
             void *opaque);
int codespan(struct buf *ob, const struct buf *text, void *opaque);
int double_emphasis(struct buf *ob, const struct buf *text, void *opaque);
int emphasis(struct buf *ob, const struct buf *text, void *opaque);
int image(struct buf *ob, const struct buf *link, const struct buf *title,
          const struct buf *alt, void *opaque);
int linebreak(struct buf *ob, void *opaque);
int link(struct buf *ob, const struct buf *link, const struct buf *title,
         const struct buf *content, void *opaque);
int raw_html_tag(struct buf *ob, const struct buf *tag, void *opaque);
int triple_emphasis(struct buf *ob, const struct buf *text, void *opaque);
int strikethrough(struct buf *ob, const struct buf *text, void *opaque);
int superscript(struct buf *ob, const struct buf *text, void *opaque);

/* low level callbacks - NULL copies input directly into the output */
void entity(struct buf *ob, const struct buf *entity, void *opaque);
void normal_text(struct buf *ob, const struct buf *text, void *opaque);

/* header and footer */
void doc_header(struct buf *ob, void *opaque);
void doc_footer(struct buf *ob, void *opaque);

void registerInterestingCallbacks(sd_callbacks &cbs);

#endif  // !_CALLBACKS_H
