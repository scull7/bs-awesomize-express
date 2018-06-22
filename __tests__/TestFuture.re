open Jest;

module Middleware = {
  type runner('a) =
    (Js.Json.t, Express.Middleware.t) => Js.Promise.t((int, 'a));

  let run: runner('a) = [%bs.raw
    "
      function (request, middleware) {
        return new Promise((resolve, reject) => {
          middleware(
            request,
            {
              status (code) {
                this.statusCode = code;
                return this;
              },
              json (data) {
                resolve([ this.statusCode, data ]);
              }
            },
            (x) => {
              reject(new Error(`Uknown: ${x}`));
            },
          )
        })
      }
    "
  ];
};

describe("Awesomize Express :: Future", () =>
  describe("make", () => {
    let schema = [|
      (
        "clientId",
        {
          read: Awesomize.Read.key("clientId"),
          sanitize: None,
          validate: [Awesomize.Validator.required],
          normalize: None,
        }: Awesomize.definition,
      ),
    |];
    let decoder = json => Json.Decode.(json |> field("clientId", int));
    let encoder = data => Json.Encode.(data |> int);
    let handler = (_, clientId) => Future.value(Belt.Result.Ok(clientId));

    testPromise("success", () => {
      let request = Json.Encode.(object_([("clientId", int(1))]));
      let middleware =
        Express.PromiseMiddleware.from(
          AwesomizeExpress.Future.make(~schema, ~decoder, ~encoder, ~handler),
        );

      Middleware.run(request, middleware)
      |> Js.Promise.then_(actual =>
           Expect.expect(actual)
           |> Expect.toEqual((200, 1))
           |> Js.Promise.resolve
         );
    });

    testPromise("fail", () => {
      let request = Json.Encode.object_([]);
      let middleware =
        Express.PromiseMiddleware.from(
          AwesomizeExpress.Future.make(~schema, ~decoder, ~encoder, ~handler),
        );
      Middleware.run(request, middleware)
      |> Js.Promise.then_(((status, _)) =>
           Expect.expect(status) |> Expect.toBe(400) |> Js.Promise.resolve
         );
    });
  })
);
