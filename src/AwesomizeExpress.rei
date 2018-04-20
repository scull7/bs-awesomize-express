let make : (
  ~schema: Awesomize.schema, 
  ~decoder: Js.Json.t => 'a, 
  ~encoder: Js.Json.t => Js.Json.t, 
  ~handler: (Express.Request.t, 'a) => Js.Promise.t(Js.Json.t),
  'c,
  Express.Request.t,
  Express.Response.t
  ) => Js.Promise.t(Express.complete);
