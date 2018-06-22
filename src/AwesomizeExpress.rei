let make:
  (
    ~schema: Awesomize.schema,
    ~decoder: Js.Json.t => 'a,
    ~encoder: 'b => Js.Json.t,
    ~handler: (Express.Request.t, 'a) => Js.Promise.t('b),
    'c,
    Express.Request.t,
    Express.Response.t
  ) =>
  Js.Promise.t(Express.complete);

module Future: {
  let make:
    (
      ~schema: Awesomize.schema,
      ~decoder: Js.Json.t => 'a,
      ~encoder: 'b => Js.Json.t,
      ~handler: (Express.Request.t, 'a) => Future.t(Belt.Result.t('b, exn)),
      'c,
      Express.Request.t,
      Express.Response.t
    ) =>
    Js.Promise.t(Express.complete);
};

module Promise: {
  let make:
    (
      ~schema: Awesomize.schema,
      ~decoder: Js.Json.t => 'a,
      ~encoder: 'b => Js.Json.t,
      ~handler: (Express.Request.t, 'a) => Js.Promise.t('b),
      'c,
      Express.Request.t,
      Express.Response.t
    ) =>
    Js.Promise.t(Express.complete);
};
