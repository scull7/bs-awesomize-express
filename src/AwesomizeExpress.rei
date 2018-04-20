let make : (
  ~schema: Awesomize.schema, 
  ~decoder: Js.Json.t => 'a, 
  ~encoder: 'b => Js.Json.t, 
  ~handler: (Express.Request.t, 'a) => Js.Promise.t('b),
  'c,
  Express.Request.t,
  Express.Response.t
  ) => Js.Promise.t(Express.complete);
